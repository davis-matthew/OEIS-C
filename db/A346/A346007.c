/*
 * A346007  Let b=5. If n == -i (mod b) for 0 <= i < b, then a(n) =
 *          binomial(b,i+1)*((n+i)/b)^(i+1).
 * https://oeis.org/A346007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let b = 5. For n ≥ 0, let r = n mod 5. Set k = 1 if r = 0,
 *            otherwise k = 6 – r (so k = 1,…,5). Let q = ⌊n/5⌋ + 1 if r ≠ 0,
 *            else q = ⌊n/5⌋. Then a(n) = C(5, k) · q^k
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 420, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* r = n mod 5 */
    idx_t r = n % 5;

    /* k = 1 if r==0 else 6‑r */
    idx_t k = (r == 0) ? 1 : 6 - r;

    /* q = floor(n/5) + (r != 0) */
    idx_t q = n / 5 + (r != 0);

    /* compute q^k */
    seq_t powk = 1;
    for (idx_t i = 0; i < k; ++i)
        powk *= (seq_t)q;

    /* binomial coefficients C(5,k) for k = 0…5 */
    static const seq_t binom[6] = {
        0,              /* C(5,0) – unused */
        (seq_t)5,       /* C(5,1) */
        (seq_t)10,      /* C(5,2) */
        (seq_t)10,      /* C(5,3) */
        (seq_t)5,       /* C(5,4) */
        (seq_t)1        /* C(5,5) */
    };

    return binom[k] * powk;
}
