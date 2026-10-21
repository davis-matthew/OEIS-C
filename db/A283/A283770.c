/*
 * A283770  Numbers k such that L(k) = 1 mod 3, where L = A000201 = lower
 *          Wythoff sequence.
 * https://oeis.org/A283770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th positive integer k such that floor(k*φ) ≡ 1 (mod 3),
   where φ = (1+√5)/2.  Floor(k*φ) is computed by integer arithmetic using a
   rational approximation of φ: φ ≈ 1618033989 / 1,000,000,000.  The approximation
   is sufficiently accurate for all k up to the required range (k ≲ 3·n). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1; below do not matter */

    const seq_t P = 1618033989ULL;   /* numerator of φ approximation   */
    const seq_t Q = 1000000000ULL;   /* denominator of φ approximation */

    idx_t k = 1;
    idx_t count = 0;

    while (1) {
        seq_t val = (seq_t)k * P / Q;          /* floor(k*φ) */
        if ((val % 3ULL) == 1ULL) {            /* check modulo 3 */
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
