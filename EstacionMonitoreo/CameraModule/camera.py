import io
import picamera
from flask import Flask, Response

app = Flask(__name__)

def generate_frames(camera):
    with io.BytesIO() as stream:
        for _ in camera.capture_continuous(stream, format='jpeg', use_video_port=True):
            stream.seek(0)
            yield (b'--frame\r\nContent-Type: image/jpeg\r\n\r\n' + stream.read() + b'\r\n')
            stream.seek(0)
            stream.truncate()

@app.route('/video_feed')
def video_feed():
    camera = picamera.PiCamera()
    camera.resolution = (640, 480)
    camera.framerate = 24
    return Response(generate_frames(camera), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, threaded=True)
