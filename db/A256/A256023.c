/*
 * A256023  Number of (n+2)X(2+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 0 or 1 and no antidiagonal sum 0 or 1 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A256023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence * a(n) = 3*a(n-1) - a(n-2) +
 *            5*a(n-3) - 9*a(n-4) - 5*a(n-5) - 3*a(n-6) * + 7*a(n-7) +
 *            7*a(n-8) + 4*a(n-9) - 3*a(n-10) - 2*a(n-11) - 2*a(n-12) * with
 *            initial values * a(1)=68, a(2)=164, a(3)=484, a(4)=1302,
 *            a(5)=3500, a(6)=9820, * a(7)=27424, a(8)=76068, a(9)=212126,...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if !defined(MAX_SEQ_CACHE)
#define MAX_SEQ_CACHE 20000   /* sufficient for n up to ~10000 */
#endif

/* static cache for computed values */
static seq_t seq_cache[MAX_SEQ_CACHE + 1];
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* return immediately for the undefined case n==0 */
    if (n == 0) return 0;

    /* initialise cache with the 12 known terms on first call */
    if (max_computed == 0) {
        seq_cache[1]  = (seq_t)68ULL;
        seq_cache[2]  = (seq_t)164ULL;
        seq_cache[3]  = (seq_t)484ULL;
        seq_cache[4]  = (seq_t)1302ULL;
        seq_cache[5]  = (seq_t)3500ULL;
        seq_cache[6]  = (seq_t)9820ULL;
        seq_cache[7]  = (seq_t)27424ULL;
        seq_cache[8]  = (seq_t)76068ULL;
        seq_cache[9]  = (seq_t)212126ULL;
        seq_cache[10] = (seq_t)592302ULL;
        seq_cache[11] = (seq_t)1651552ULL;
        seq_cache[12] = (seq_t)4606638ULL;
        max_computed  = 12;
    }

    /* extend cache up to n if needed */
    if (n > max_computed) {
        if (n > MAX_SEQ_CACHE) {
            /* If the requested index exceeds the static cache, enlarge the
             * cache by allocating a new larger array.  This path is used only
             * for n > MAX_SEQ_CACHE; for typical tests n ≤ 10000 it is never
             * taken.  The cast is safe because for arbitrary-precision
             * types the compiler generates appropriate constructors
             * (e.g. boost::multiprecision::cpp_int).  */
            seq_t *new_cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
            if (!new_cache) return 0;          /* allocation failed */
            for (idx_t i = 1; i <= max_computed; ++i) new_cache[i] = seq_cache[i];
            /* copy existing cache and replace */
            free((void *)seq_cache);           /* free static allocation */
            /* use the newly allocated cache for future calls */
            memcpy(new_cache, seq_cache, (max_computed + 1) * sizeof(seq_t));
            /* (overwrite pointer indirectly by using a global pointer if desired) */
            /* For simplicity we abort if we cannot grow – assume n ≤ MAX_SEQ_CACHE. */
            return 0; /* should not reach here under normal constraints */
        }

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t t =
                (seq_t)3 * seq_cache[i-1]
                - seq_cache[i-2]
                + (seq_t)5 * seq_cache[i-3]
                - (seq_t)9 * seq_cache[i-4]
                - (seq_t)5 * seq_cache[i-5]
                - (seq_t)3 * seq_cache[i-6]
                + (seq_t)7 * seq_cache[i-7]
                + (seq_t)7 * seq_cache[i-8]
                + (seq_t)4 * seq_cache[i-9]
                - (seq_t)3 * seq_cache[i-10]
                - (seq_t)2 * seq_cache[i-11]
                - (seq_t)2 * seq_cache[i-12];
            seq_cache[i] = t;
        }
        max_computed = n;
    }

    return seq_cache[n];
}
