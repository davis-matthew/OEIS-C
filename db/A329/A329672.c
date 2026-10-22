/*
 * A329672  Number of meanders of length n with Motzkin-steps avoiding the
 *          consecutive steps UU.
 * https://oeis.org/A329672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A329672
 * Number of meanders of length n with Motzkin-steps avoiding consecutive UU.
 *
 * Recurrence (n ≥ 7):
 *   (n+2)a(n) + (-3n-5)a(n-1) + (-3n+2)a(n-2) + (5n+2)a(n-3)
 *   + (11n-19)a(n-4) + (9n-32)a(n-5) + 2a(n-6) + 2(-n+6)a(n-7) = 0
 *
 * Initial values:
 *   a(0)=1, a(1)=2, a(2)=4, a(3)=9, a(4)=20, a(5)=46, a(6)=107
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* small initial block */
static const seq_t _a_init[7] = {1,2,4,9,20,46,107};

/* compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* direct return for the first seven terms */
    if (n < 7) return _a_init[n];

    /* create a temporary array large enough for n+1 terms */
#ifdef __cplusplus
    std::vector<seq_t> cache(static_cast<size_t>(n)+1);
#else
    seq_t *cache = (seq_t*)malloc(((size_t)n+1)*sizeof(seq_t));
    if (!cache) abort();
#endif

    /* initialise the known values */
    for (idx_t i = 0; i < 7; ++i) cache[i] = _a_init[i];

    /* iterate from 7 up to n, filling the array */
    for (idx_t j = 7; j <= n; ++j)
    {
        seq_t sum = 0;

        /* the terms on the right‑hand side of the recurrence */
        sum += (seq_t)(3*j+5) * cache[j-1];   /* (3n+5)a(n-1)   */
        sum += (seq_t)(3*j-2) * cache[j-2];   /* (3n-2)a(n-2)   */
        sum -= (seq_t)(5*j+2) * cache[j-3];   /* -(5n+2)a(n-3)  */
        sum -= (seq_t)(11*j-19) * cache[j-4]; /* -(11n-19)a(n-4)*/
        sum -= (seq_t)(9*j-32) * cache[j-5];  /* -(9n-32)a(n-5) */
        sum -= (seq_t)2 * cache[j-6];         /* -2a(n-6)        */
        sum += (seq_t)(2*j-12) * cache[j-7];  /* +(2n-12)a(n-7)  */

        cache[j] = sum / (seq_t)(j+2);        /* a(n) = sum/(n+2) */
    }

    seq_t result = cache[n];

#ifdef __cplusplus
    /* vector cleans up automatically */
#else
    free(cache);
#endif

    return result;
}
