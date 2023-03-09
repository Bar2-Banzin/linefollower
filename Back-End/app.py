from flask import Flask, request, jsonify
import werkzeug
import threading
import time
import os
 
from queue import Queue


app = Flask(__name__)
global results
results = [False,False,False] 

UPLOAD_FOLDER = 'static/uploads/'
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER


queue=Queue()

def task(results,index):
 
    """
    background Process handled by Threads
    :return: None
    """
    print("Started Task ...")
    print(threading.current_thread().name)
    time.sleep(.5)

    filename=queue.get()

    print(filename + "============================")
    os.remove('./zeinb/'+filename)
    results[index] = 0
    print("completed .....")
 

@app.route('/')
def main():
    return 'Homepage'


@app.route('/upload', methods=['POST'])
def upload():
    if (request.method == "POST"):
        global results
        print(results)
        imagefile = request.files['image']
        print(imagefile)
        filename = werkzeug.utils.secure_filename(imagefile.filename)
        imagefile.save("./zeinb/"+filename)
        queue.put(filename)
        if(queue.qsize()>=3 and not any(results)):
            results = False
            for i in range(3):
                threading.Thread(target=task,args=( results,i,)).start()
       
        # imagefile.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        return jsonify({
            "message": "Image Uploaded Successfully"
        })


if __name__ == '__main__':
    app.run(host="192.168.137.1", port=5000, debug=False)