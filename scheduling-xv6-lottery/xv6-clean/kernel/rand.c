unsigned long _Randseed = 1;
const int RAND_MAX = 32767;

int rand_int(int n) {
  _Randseed = _Randseed * 1103515245 + 12345;
  int rand = ((unsigned int)(_Randseed >> 16) & RAND_MAX);
  return rand % n;
}

void srand_int(unsigned int seed) { _Randseed = seed; }
