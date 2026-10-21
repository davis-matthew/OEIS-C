/*
 * A258256  Expansion of f(q^3) * psi(-q^3)^3 / (psi(-q) * psi(-q^9)) in
 *          powers of q where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A258256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers of n, with * a(0) = 1, * a(2^e)
 *            = 1, * a(3^e) = 2 * (1 + (-1)^e), * a(p^e) = (1 + (-1)^e) / 2 if
 *            p ≡ 3 (mod 4) and p ≠ 3, * a(p^e) = e + 1 if p ≡ 1 (mod 4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t nn = n;

    /* Skip all factors of 2 – they contribute 1. */
    while ((nn & 1ULL) == 0) nn >>= 1;

    /* Factor remaining odd part. */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        if (nn % p == 0) {
            uint64_t exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            seq_t mult;
            if (p == 3ULL) {
                /* a(3^e) = 2 * (1 + (-1)^e) */
                mult = ((exp & 1ULL) == 0) ? (seq_t)4 : (seq_t)0;
            } else if ((p & 3ULL) == 1) {
                /* a(p^e) = e + 1 for p ≡ 1 (mod 4) */
                mult = (seq_t)(exp + 1);
            } else { /* p ≡ 3 (mod 4) and p ≠ 3 */
                /* a(p^e) = (1 + (-1)^e) / 2 */
                mult = ((exp & 1ULL) == 0) ? (seq_t)1 : (seq_t)0;
            }
            result *= mult;
            if (result == 0) return result; /* early exit if zero product */
        }
    }

    /* If something remains, it is a prime > sqrt(original n). */
    if (nn > 1) {
        uint64_t p = nn;
        uint64_t exp = 1;
        seq_t mult;
        if (p == 3ULL) {
            mult = ((exp & 1ULL) == 0) ? (seq_t)4 : (seq_t)0;
        } else if ((p & 3ULL) == 1) {
            mult = (seq_t)(exp + 1);
        } else { /* p ≡ 3 (mod 4) and p ≠ 3 */
            mult = ((exp & 1ULL) == 0) ? (seq_t)1 : (seq_t)0;
        }
        result *= mult;
    }

    return result;
}
