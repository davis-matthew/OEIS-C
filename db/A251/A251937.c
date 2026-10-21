/*
 * A251937  Number of length 3+2 0..n arrays with the sum of the maximum minus
 *          the median of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A251937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 151, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A251937 computed by the given empirical recurrence:
   a(1)=18, a(2)=115, a(3)=431, a(4)=1191, a(5)=2695,
   a(6)=5340, a(7)=9615, a(8)=16098, a(9)=25474, a(10)=38538,
   a(11)=56176
   a(n) = 3*a(n‑1) – 2*a(n‑2) – 3*a(n‑4) + 3*a(n‑5) + 3*a(n‑6)
          – 3*a(n‑7) – 2*a(n‑9) + 3*a(n‑10) – a(n‑11)     (n≥12)
   The function is memoised; the cache size is fixed to 10001,
   sufficient for the required n≤10000. */

#include <stdbool.h>   /* for bool */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* static cache – 1‑based indexing; a[0] left 0 */
    static seq_t cache[10001];
    static bool initialized = false;
    static idx_t max_cached = 0;

    if (!initialized) {
        /* initialise first 11 terms explicitly (a[0] unused) */
        cache[1]  = (seq_t)18;
        cache[2]  = (seq_t)115;
        cache[3]  = (seq_t)431;
        cache[4]  = (seq_t)1191;
        cache[5]  = (seq_t)2695;
        cache[6]  = (seq_t)5340;
        cache[7]  = (seq_t)9615;
        cache[8]  = (seq_t)16098;
        cache[9]  = (seq_t)25474;
        cache[10] = (seq_t)38538;
        cache[11] = (seq_t)56176;
        max_cached = 11;
        initialized = true;
    }

    /* extend cache if necessary (up to 10000 requested) */
    if (n > max_cached) {
        /* use only up to the allowed maximum */
        if (n > 10000)
            n = 10000;   /* clamp to cache size – caller may ignore >10000 */

        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = ((seq_t)3) * cache[i - 1]
                     - ((seq_t)2) * cache[i - 2]
                     - ((seq_t)3) * cache[i - 4]
                     + ((seq_t)3) * cache[i - 5]
                     + ((seq_t)3) * cache[i - 6]
                     - ((seq_t)3) * cache[i - 7]
                     - ((seq_t)2) * cache[i - 9]
                     + ((seq_t)3) * cache[i -10]
                     - cache[i - 11];
        }
        max_cached = n;
    }

    /* return the cached value (or the clamped value for large n) */
    return cache[n];
}
