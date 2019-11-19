/*
Copyright (c) 2012-2016 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Python.h"
#include "PCA9685.h"

#define MIN_VAL 100
#define MAX_VAL 500

static int fd;
static int addr = 0x40;
static unsigned int setOnVals[_PCA9685_CHANS] =
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static unsigned int setOffVals[_PCA9685_CHANS] =
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int initHardware(int adpt, int addr, int freq)
{
	int afd = PCA9685_openI2C(adpt, addr);
	PCA9685_initPWM(afd, addr, freq);
	return afd;
}
static PyObject *init(PyObject *self, PyObject *args)
{
	int adpt = 0;
	int freq = 50 + 1;
	int i = 0;

	if (!PyArg_ParseTuple(args, "ii", &adpt, &freq)) {
		printf("invalid args");
		return NULL;
	}

	printf("adpt = %d, freq = %d\n", adpt, freq);
	for (i = 0; i < _PCA9685_CHANS; i++)
	{
		setOffVals[i] = 0;
	}

	fd = initHardware(adpt, addr, freq);
	printf("fd = %d\n", fd);

	Py_RETURN_NONE; //No retornamos nada
}

static PyObject *set_pwm(PyObject *self, PyObject *args)
{
	int servo[2];

	if (!PyArg_ParseTuple(args, "ii", &servo[0], &servo[1]))
	{
		printf("invalid args");
		return NULL;
	}
	printf("api servo=%d,%d\n", servo[0], servo[1]);

	setOffVals[0] = servo[0];
	setOffVals[1] = servo[1];

	int result = PCA9685_setPWMVals(fd, addr, setOnVals, setOffVals);
	printf("api result=%d\n",result, fd);

	Py_RETURN_NONE; //No retornamos nada
}

static PyMethodDef pca9685_methods[] =
		{
				{"init", init, METH_VARARGS, "initialize the pca9685 board"},
				{"set", set_pwm, METH_VARARGS, "set the values for the pwm output"},
				{NULL, NULL, 0, NULL}};

PyMODINIT_FUNC initpca9685(void)
{
	(void)Py_InitModule("pca9685", pca9685_methods);
}