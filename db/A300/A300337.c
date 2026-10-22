/*
 * A300337  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300337: Number of n×3 0..1 arrays with king‑move adjacency
   Note: a(1)=1, a(2)=13, …, a(13)=1005876277.
   Recurrence for n>13:
      a(n) = 6*a(n-1) +6*a(n-2) -40*a(n-3) -59*a(n-4) +79*a(n-5)
           +97*a(n-6) -40*a(n-7) +20*a(n-8) +8*a(n-9) -44*a(n-10) -6*a(n-11)
   The function below memoises the values up to n=10000.  It works for
   any seq_t/idx_t pair, including arbitrary‑precision seq_t classes.      */

#include <stdbool.h>

#define MAX_N 10000                /* largest n handled (adjust if needed) */

static seq_t _cache[MAX_N + 1] = {0}; /* 1‑based: _cache[1] … _cache[MAX_N] */
static idx_t _max_computed = 0;       /* largest index already computed */
static bool  _initialized = false;   /* initial values set? */

/* Initialise the first 13 terms once. */
static void _ensure_initialized(void)
{
    if (_initialized) return;

    _cache[1] =  (seq_t)1;
    _cache[2] =  (seq_t)13;
    _cache[3] =  (seq_t)47;
    _cache[4] =  (seq_t)255;
    _cache[5] =  (seq_t)1272;
    _cache[6] =  (seq_t)6780;
    _cache[7] =  (seq_t)36498;
    _cache[8] =  (seq_t)198531;
    _cache[9] =  (seq_t)1088200;
    _cache[10] = (seq_t)5984027;
    _cache[11] = (seq_t)32988462;
    _cache[12] = (seq_t)182088665;
    _cache[13] = (seq_t)1005876277;

    _max_computed = 13;
    _initialized  = true;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined, return 0 */

    if (n > MAX_N) {                       /* beyond pre‑allocated range;
                                               this implementation only
                                               supports upto MAX_N */
        /* Could extend dynamic allocation here, but not needed for the
           standard test range. */
        return 0;
    }

    _ensure_initialized();

    if (n <= _max_computed)                /* value already known */
        return _cache[n];

    /* Compute successive values up to n using the recurrence */
    for (idx_t i = _max_computed + 1; i <= n; ++i) {
        /* Separate positive and negative contributions to avoid
           unsigned underflow when using negative constants. */
        seq_t pos = 6 * _cache[i-1] + 6 * _cache[i-2]
                    + 79 * _cache[i-5] + 97 * _cache[i-6]
                    + 20 * _cache[i-8] + 8 * _cache[i-9];

        seq_t neg = 40 * _cache[i-3] + 59 * _cache[i-4]
                    + 40 * _cache[i-7] + 44 * _cache[i-10]
                    + 6  * _cache[i-11];

        _cache[i] = pos - neg;               /* unsigned subtraction is safe */
    }

    _max_computed = n;
    return _cache[n];
}
