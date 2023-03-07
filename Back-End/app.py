from flask import Flask, request, jsonify
import os
import werkzeug

app = Flask(__name__)


UPLOAD_FOLDER = 'static/uploads/'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER


@app.route('/')
def main():
    return 'Homepage'


@app.route('/upload', methods=['POST'])
def upload():
    if (request.method == "POST"):
        imagefile = request.files['image']
        print(imagefile)
        filename = werkzeug.utils.secure_filename(imagefile.filename)
        imagefile.save("./"+filename)
        # imagefile.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        return jsonify({
            "message": "Image Uploaded Successfully"
        })


if __name__ == '__main__':
    app.run(host="192.168.137.1", port=5000, debug=True)