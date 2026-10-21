/*
 * A276383  Complement of A158919: complementary Beatty sequence to the Beatty
 *          sequence defined by the tribonacci constant tau = A058265.
 * https://oeis.org/A276383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * tau_prime) where * tau_prime = (1 + cbrt(19 +
 *            3*sqrt(33)) + cbrt(19 - 3*sqrt(33))) / * (-2 + cbrt(19 +
 *            3*sqrt(33)) + cbrt(19 - 3*sqrt(33))) ≈ *
 *            2.191487883953118747061354268227517294
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use a fixed‑precision rational approximation of tau_prime:
 *   NUM / DEN  ≈ 2.191487883953118747
 * with DEN = 1e18 and NUM = floor(tau_prime * DEN).
 * The error is < 1e-18, so for any n ≤ 10 000 we have
 * |n * (tau_prime - NUM/DEN)| < 10⁻¹⁴ < 1, thus
 * floor(n * NUM/DEN) = floor(n * tau_prime).          */
static const seq_t NUM = 2191487883953118747ULL;  /* floor(tau_prime * 1e18) */
static const seq_t DEN = 1000000000000000000ULL;  /* 1e18 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* use 128‑bit arithmetic to avoid overflow of the product */
    return (seq_t)((unsigned __int128)nn * NUM / DEN);
}
