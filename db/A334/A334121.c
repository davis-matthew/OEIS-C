/*
 * A334121  a(n) is the multiplicative inverse of A008514(n) modulo
 *          A008514(n+1).
 * https://oeis.org/A334121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008514(n)^(-1) mod A008514(n+1), where A008514(n) = n^4
 *            + (n+1)^4
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A008514_a(idx_t n)
{
    seq_t nn  = (seq_t)n;
    seq_t nn1 = nn + 1;

    seq_t nn_sq  = nn * nn;
    seq_t nn4    = nn_sq * nn_sq;
    seq_t nn1_sq = nn1 * nn1;
    seq_t nn1_4  = nn1_sq * nn1_sq;

    return nn4 + nn1_4;
}

/* SHARED_HELPER: modular inverse via extended Euclid using only unsigned arithmetic */
static seq_t A334121_modinv(seq_t a, seq_t m)
{
    seq_t r0 = m, r1 = a % m;
    seq_t t0 = 0, t1 = 1;   /* magnitudes of Bezout coefficients; signs alternate */
    unsigned long long k = 0;
    while (r1 != 0) {
        seq_t q = r0 / r1;
        seq_t r2 = r0 - q * r1;
        seq_t t2 = t0 + q * t1;
        r0 = r1; r1 = r2;
        t0 = t1; t1 = t2;
        k++;
    }
    /* r0 is gcd (1); t0 is coefficient of index k, positive iff k is odd */
    if (k % 2 == 1) return t0 % m;
    return (m - t0 % m) % m;
}

seq_t a(idx_t n)
{
    seq_t x = A008514_a(n);
    seq_t m = A008514_a(n + 1);
    return A334121_modinv(x, m);
}
