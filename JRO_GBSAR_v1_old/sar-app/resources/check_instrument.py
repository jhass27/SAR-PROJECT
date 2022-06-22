from flask_restful import Resource
from flask import jsonify

import os, sys

sys.path.append(os.path.abspath(os.path.join(os.getcwd(), os.pardir)))

from common.common_functions import *
from static.constants import *

class CheckInstrument(Resource):
	def get(self, instrument_name):
		status = 'online' if ping(HOST_LIST[instrument_name]) else 'offline'
		return jsonify(instrument_name = instrument_name, status = status)
