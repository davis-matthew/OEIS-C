/*
 * A375079  a(n) = a(n-1) + a(n-2) + ... + a(n-k) where k = (a(n-1) mod (n-1))
 *          + 1 for n >= 3, with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A375079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + … + a(n-k) where k = (a(n-1) mod (n-1)) + 1,
 *            with a(1)=1, a(2)=2. (OEIS A375079)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>

#define MAX_N 20000   /* enough for all tests (n≈10000) */

seq_t a(idx_t n)
{
    static seq_t cache[MAX_N + 1];
    static seq_t prefix[MAX_N + 1];
    static idx_t computed = 0;   /* highest n already computed */

    if (n == 0) return 0;   /* safety for out‑of‑range calls */
    assert(n <= MAX_N);

    if (n > computed) {
        /* initialise prefix[0] only once */
        if (computed == 0) prefix[0] = (seq_t)0;

        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
                prefix[1] = cache[1];
            } else if (i == 2) {
                cache[2] = (seq_t)2;
                prefix[2] = prefix[1] + cache[2];
            } else {
                /* k = (a(i-1) mod (i-1)) + 1 */
                seq_t k_val = cache[i - 1] % ( (seq_t)(i - 1) ) + 1;
                idx_t k = (idx_t)k_val;

                seq_t sum = prefix[i - 1];
                if (k < i - 1) {
                    sum -= prefix[i - 1 - k];
                }   /* else k == i-1: sum already equals prefix[i-1]-prefix[0] */

                cache[i] = sum;
                prefix[i] = prefix[i - 1] + sum;
            }
        }
        computed = n;
    }
    return cache[n];
}
