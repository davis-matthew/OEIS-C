/*
 * A295319  a(n) is the sum of all n-digit palindromes.
 * https://oeis.org/A295319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 45; a(2) = 495; a(3) = 49500; For n > 3, a(n) = 1000 *
 *            a(n-2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1u) return (seq_t)45;
    if (n == 2u) return (seq_t)495;
    if (n == 3u) return (seq_t)49500;

    seq_t result;
    idx_t muls;   /* number of times to multiply by 1000 */

    /* Even indices use the 495 starting value; odd indices use 49500. */
    if ((n & 1u) == 0u) {
        result = (seq_t)495;
        muls   = (n / 2u) - 1u;          /* n = 2 + 2*k  -> k = n/2 - 1 */
    } else {
        result = (seq_t)49500;
        muls   = (n - 3u) / 2u;          /* n = 3 + 2*k  -> k = (n-3)/2 */
    }

    /* Multiply by 1000 repeatedly.  Overflow for values beyond the
       capacity of seq_t is acceptable, because such terms are not
       evaluated by the 64‑bit build. */
    for (idx_t i = 0; i < muls; ++i)
        result *= (seq_t)1000;

    return result;
}
