/*
 * A275038  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-2) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence * a(n) = 3*a(n-1) - 2*a(n-2) +
 *            4*a(n-3) + a(n-4) + 3*a(n-5) + * 8*a(n-6) + a(n-7) - 3*a(n-8) -
 *            a(n-9) (n > 10) * with initial values: * a(1)=3, a(2)=18,
 *            a(3)=46, a(4)=137, a(5)=401, * a(6)=1152, a(7)=3336, a(8)=9720,
 *            a(9)=28277, a(10)=82174
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* --------------  helper functions for dynamic cache  -------------- */
static seq_t *cache   = NULL;   /* pointer to cached terms a(1)...a(n) */
static size_t capacity = 0;    /* allocated array size          */
static size_t computed = 0;    /* number of terms actually computed */

/*  enlarge cache to hold at least 'need' terms */
static void
ensure_capacity(size_t need)
{
    if (capacity >= need)
        return;

    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp)
        exit(1);                 /* out of memory */

    cache = tmp;
    capacity = new_cap;
}

/*  initialise cache with the first 10 terms of the sequence  */
static void
init_cache(void)
{
    ensure_capacity(10);
    cache[0] = (seq_t)3;
    cache[1] = (seq_t)18;
    cache[2] = (seq_t)46;
    cache[3] = (seq_t)137;
    cache[4] = (seq_t)401;
    cache[5] = (seq_t)1152;
    cache[6] = (seq_t)3336;
    cache[7] = (seq_t)9720;
    cache[8] = (seq_t)28277;
    cache[9] = (seq_t)82174;
    computed = 10;
}

/*  public function: a(n) for n >= 1  */
seq_t a(idx_t n)
{
    if (n < 1)    /* undefined, return zero just in case */
        return (seq_t)0;

    /* use size_t for indexing – safe because n ≤ IDX_MAX, fits in size_t */
    size_t N = (size_t)n;

    /* initialise cache on first call */
    if (computed == 0)
        init_cache();

    /* extend cache if necessary */
    if (N > computed) {
        ensure_capacity(N);
        for (size_t i = computed; i < N; ++i) {
            /* recurrence valid for n > 10 => i >= 10 (zero‑based i = n-1) */
            seq_t val =
                (seq_t)3        * cache[i-1]          /* 3*a(n-1) */
              - (seq_t)2        * cache[i-2]          /* -2*a(n-2) */
              + (seq_t)4        * cache[i-3]          /* +4*a(n-3) */
              +                 cache[i-4]            /* +a(n-4) */
              + (seq_t)3        * cache[i-5]          /* +3*a(n-5) */
              + (seq_t)8        * cache[i-6]          /* +8*a(n-6) */
              +                 cache[i-7]            /* +a(n-7) */
              - (seq_t)3        * cache[i-8]          /* -3*a(n-8) */
              -                 cache[i-9];           /* -a(n-9) */
            cache[i] = val;
        }
        computed = N;
    }

    return cache[N-1];   /* cache is zero‑based, n is 1‑based */
}
