/*
 * A384114  Consecutive states of the linear congruential pseudo-random number
 *          generator (125*s+1) mod 2^12 when started at s=1.
 * https://oeis.org/A384114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (125*a(n-1) + 1) mod 2^12 (a(1) = 1)
 * Range:     u8 none, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period is 4096; precompute once */
    static int initialized = 0;
    static seq_t cache[4096];

    if (!initialized) {
        const seq_t modulus = (seq_t)1 << 12;      /* 2^12 = 4096 */
        cache[0] = (seq_t)1;                      /* a(1) = 1 */
        for (idx_t i = 1; i < 4096; ++i) {
            cache[i] = ((seq_t)125 * cache[i - 1] + 1) % modulus;
        }
        initialized = 1;
    }

    idx_t idx = (n - 1) % 4096;   /* 0‑based index into the period */
    return cache[idx];
}
