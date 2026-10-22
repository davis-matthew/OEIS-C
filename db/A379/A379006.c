/*
 * A379006  Ordinal transform of A355582, where A355582 is the largest
 *          5-smooth divisor of n.
 * https://oeis.org/A379006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { k | 1 ≤ k ≤ floor(n / l5(n)), gcd(k,30)=1 }
 * Range:     u8 n <= 958, u16 n <= 9551, u32 n <= 9551, u64 n <= 9551,
 *            u128 n <= 9551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where l5(n) is the largest 5‑smooth divisor of n. */
seq_t a(idx_t n)
{
    /* compute the largest 5‑smooth divisor l5(n) */
    idx_t v = n;
    idx_t val = 1;
    while ((v & 1) == 0) {            /* factor 2 */
        val *= 2;
        v >>= 1;
    }
    while (v % 3 == 0) {              /* factor 3 */
        val *= 3;
        v /= 3;
    }
    while (v % 5 == 0) {              /* factor 5 */
        val *= 5;
        v /= 5;
    }

    /* X = floor(n / val) */
    idx_t X = n / val;

    /* count numbers ≤ X that are coprime to 30
       using inclusion–exclusion for the primes 2,3,5 */
    idx_t cnt = X;
    cnt -= X / 2;
    cnt -= X / 3;
    cnt -= X / 5;
    cnt += X / 6;
    cnt += X / 10;
    cnt += X / 15;
    cnt -= X / 30;

    return (seq_t)cnt;
}
