/*
 * A263832  The number c_{Cc,pi_1(B_2)}(n) of the second amphicosm n-coverings
 *          over the second amphicosm.
 * https://oeis.org/A263832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 8880, u32 n <= 9308, u64 n <= 9308,
 *            u128 n <= 9308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A263832
 * 
 * a(n) = product of contributions from each prime power in n.
 * Let n = 2^k * Π_{p odd} p^e.
 *
 *  • For the 2‑part:  f(2^k) = 1   (k = 0),
 *                     f(2^k) = 2^k – 2   (k ≥ 1).
 *
 *  • For an odd prime power p^e (e ≥ 1):
 *       f(p^e) = (p^(e+2) – (e+2)p + (e+1)) / (p‑1)^2
 *              = p^e + 2p^(e‑1) + 3p^(e‑2) + … + (e+1).
 *
 * The function multiplies those contributions to obtain a(n).
 * Zero occurs exactly when n ≡ 2 (mod 4) (k = 1), because
 * f(2) = 0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, though n≥1 by definition */

    /* Separate powers of 2 */
    idx_t k = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        k++;
    }
    idx_t odd = n;                    /* remaining odd part */
    seq_t result = 1;

    /* Factor the odd part */
    for (idx_t p = 3; p * p <= odd; p += 2) {
        if (odd % p == 0) {
            idx_t e = 0;
            while (odd % p == 0) {
                odd /= p;
                e++;
            }
            /* Compute f(p^e) = (p^(e+2) - (e+2)p + (e+1)) / (p-1)^2 */
            seq_t num = 1;
            for (idx_t i = 0; i < e + 2; ++i) {
                num *= (seq_t)p;
            }
            num -= ((seq_t)(e + 2)) * (seq_t)p;
            num += (seq_t)(e + 1);
            seq_t denom = (seq_t)(p - 1) * (seq_t)(p - 1);
            seq_t part = num / denom;
            result *= part;
        }
    }

    /* If an odd prime factor remains */
    if (odd > 1) {
        /* odd is prime, e = 1 ⇒ f(odd) = odd + 2 */
        result *= ((seq_t)odd + 2);
    }

    /* Multiply by the 2-part contribution */
    if (k > 0) {
        seq_t pow2 = ((seq_t)1 << k) - 2;   /* 2^k - 2 (k≥1) */
        result *= pow2;
    }

    return result;
}
