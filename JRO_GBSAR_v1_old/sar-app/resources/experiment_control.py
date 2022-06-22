from flask_restful import Resource
from flask import jsonify, request
from datetime import datetime

import os, sys

sys.path.append(os.path.abspath(os.path.join(os.getcwd(), os.pardir)))

from common import db
from static.sar_experiment import *
from config import *
from MyGlobals import myglobals

class ExperimentControl(Resource):
	def get(self, instruction):
		if instruction=='start' and myglobals.experiment==None:
			myglobals.experiment=sar_experiment()
			myglobals.experiment.start()
			return jsonify('Experiment started running')
		elif instruction=='start' and myglobals.experiment.isAlive():
			return jsonify('An experiment is running')

		if instruction=='stop' and myglobals.experiment!=None:
			if myglobals.experiment.isAlive():
				myglobals.experiment.stop()
			else:
				return jsonify('Experiment already stopped')

		if instruction=='status':
			#query for status
			#return jsonify(status=status)
			pass
