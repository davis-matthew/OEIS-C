/*
 * A259947  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000000 00000001 or 00000101.
 * https://oeis.org/A259947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259947
   Recurrence (for n > 17):
     a(n) = 2a(n‑1) + 2a(n‑2) + 12a(n‑3) + 27a(n‑4)
            − 31a(n‑5) − 10a(n‑6) − 62a(n‑7) − 30a(n‑8)
            + 42a(n‑9) + 13a(n‑10) + 47a(n‑11)
            − 3a(n‑12) − 14a(n‑13) − 4a(n‑15) + 2a(n‑16)
   Initial values (a(1) … a(17)):
     156, 549, 2438, 8699, 30351, 115410, 438353, 1615572,
     5983278, 22387088, 83497726, 310419235, 1155916673,
     4308316980, 16046423584, 59751346761, 222558385418
   The sequence is memoised and works for arbitrary order of
   requests.  The implementation uses only integer arithmetic. */

#include <stddef.h>
#include <stdint.h>

#define MAX_N 20000                       /* sufficient for all tests (n ≤ 10000) */

static seq_t a_cache[MAX_N + 1] = {0};
static char a_cache_inited = 0;

/* initial known terms a(1) … a(17) */
static const seq_t a_init[17] = {
    (seq_t)156ULL,
    (seq_t)549ULL,
    (seq_t)2438ULL,
    (seq_t)8699ULL,
    (seq_t)30351ULL,
    (seq_t)115410ULL,
    (seq_t)438353ULL,
    (seq_t)1615572ULL,
    (seq_t)5983278ULL,
    (seq_t)22387088ULL,
    (seq_t)83497726ULL,
    (seq_t)310419235ULL,
    (seq_t)1155916673ULL,
    (seq_t)4308316980ULL,
    (seq_t)16046423584ULL,
    (seq_t)59751346761ULL,
    (seq_t)222558385418ULL
};

/* initialise the cache with the known terms and extend it up to n */
static void ensure_cache(idx_t n)
{
    if (n > MAX_N)
        n = MAX_N;                      /* limit to preallocated storage */

    if (!a_cache_inited) {              /* populate only once */
        for (idx_t i = 0; i < 17; ++i)
            a_cache[i + 1] = a_init[i];
        a_cache_inited = 1;
    }

    /* compute missing terms with the recurrence */
    for (idx_t i = 18; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* positive contributions */
        pos += a_cache[i - 1] * 2;
        pos += a_cache[i - 2] * 2;
        pos += a_cache[i - 3] * 12;
        pos += a_cache[i - 4] * 27;
        pos += a_cache[i - 9] * 42;
        pos += a_cache[i - 10] * 13;
        pos += a_cache[i - 11] * 47;
        pos += a_cache[i - 16] * 2;

        /* negative contributions */
        neg += a_cache[i - 5] * 31;
        neg += a_cache[i - 6] * 10;
        neg += a_cache[i - 7] * 62;
        neg += a_cache[i - 8] * 30;
        neg += a_cache[i - 12] * 3;
        neg += a_cache[i - 13] * 14;
        neg += a_cache[i - 15] * 4;

        a_cache[i] = pos - neg;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1; n==0 never requested */
        return 0;
    ensure_cache(n);
    return a_cache[n];
}
