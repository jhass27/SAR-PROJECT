from flask_restful import Resource
from flask import jsonify, request

import os, sys

sys.path.append(os.path.abspath(os.path.join(os.getcwd(), os.pardir)))

from MyGlobals import myglobals

class InstanceTest(Resource):
    def get(self, instruction):
        if instruction=='hola':
            myglobals.counter=myglobals.counter+1

        if instruction=='chau':
            pass
