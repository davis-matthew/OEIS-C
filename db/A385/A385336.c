/*
 * A385336  Consecutive states of the linear congruential pseudo-random number
 *          generator (1541*s + 2957) mod 14000 when started at s=1.
 * https://oeis.org/A385336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (1541 * a(n-1) + 2957) mod 14000, with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of the sequence is 14000. */

seq_t a(idx_t n)
{
    /* Static cache for one full period */
    static seq_t cache[14000];
    static int initialized = 0;            /* 0=not yet, 1=done */
    static const seq_t M   = (seq_t)14000;
    static const seq_t A   = (seq_t)1541;
    static const seq_t B   = (seq_t)2957;

    if (!initialized) {
        /* Seed with the first term */
        cache[0] = (seq_t)1;
        /* Generate the remaining 13999 terms */
        for (idx_t i = 1; i < 14000; ++i) {
            /* Compute (1541 * prev + 2957) mod 14000 */
            seq_t t = cache[i-1];
            seq_t val = (t * A + B) % M;
            cache[i] = val;
        }
        initialized = 1;
    }

    /* Convert n to 0‑based index modulo the period */
    idx_t idx = (n - 1) % (idx_t)14000;
    return cache[idx];
}
