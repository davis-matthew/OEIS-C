/*
 * A378009  a(n) = b(7*n+1), with the sequence {b(n)} having Dirichlet g.f.
 *          Product_{chi} L(chi,s), where chi runs through all Dirichlet
 *          characters modulo 7; 7th column of A378007.
 * https://oeis.org/A378009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(7*n+1), where b(n) is multiplicative: * b(7^e) = 0; *
 *            if p≡1 (mod 7): b(p^e) = C(e+5,5); * if p≡6 (mod 7): b(p^e) =
 *            C(e/2+2,2) for even e, 0 otherwise; * if p≡2,4 (mod 7): b(p^e) =
 *            e/3 + 1 for e≡0 (mod 3), 0 otherwise; * if p≡3,5 (mod 7): b(p^e)
 *            = 1 for e≡0 (mod 6), 0 otherwise
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = 7*n + 1 */
    idx_t m = 7 * n + 1;
    if (m == 1) return 1;            /* b(1) = 1 */
    /* The following early check is never true for m = 7*n+1, but keeps
       the implementation general. */
    if (m % 7 == 0) return 0;

    seq_t result = 1;
    idx_t temp = m;

    /* factor out 2 */
    if (temp % 2 == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        seq_t contrib = (e % 3 == 0) ? (e / 3 + 1) : 0;
        if (contrib == 0) return 0;
        result *= contrib;
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t contrib = 0;
            idx_t mod = p % 7;
            if (mod == 1) {
                /* binomial(e+5,5) */
                seq_t prod = 1;
                for (idx_t k = 0; k < 5; ++k)
                    prod *= (seq_t)(e + 5 - k);
                prod /= 120; /* 5! = 120 */
                contrib = prod;
            } else if (mod == 6) {
                if ((e & 1) == 0) {
                    seq_t k = e / 2;
                    contrib = (k + 2) * (k + 1) / 2;
                } else
                    contrib = 0;
            } else if (mod == 2 || mod == 4) {
                if (e % 3 == 0) contrib = e / 3 + 1;
                else contrib = 0;
            } else if (mod == 3 || mod == 5) {
                if (e % 6 == 0) contrib = 1;
                else contrib = 0;
            }
            if (contrib == 0) return 0;
            result *= contrib;
        }
    }

    /* remaining prime factor > sqrt(original m) */
    if (temp > 1) {
        idx_t p = temp;
        idx_t e = 1;
        seq_t contrib = 0;
        idx_t mod = p % 7;
        if (mod == 1) {
            seq_t prod = 1;
            for (idx_t k = 0; k < 5; ++k)
                prod *= (seq_t)(e + 5 - k);
            prod /= 120;
            contrib = prod;
        } else if (mod == 6) {
            if ((e & 1) == 0) {
                seq_t k = e / 2;
                contrib = (k + 2) * (k + 1) / 2;
            } else
                contrib = 0;
        } else if (mod == 2 || mod == 4) {
            if (e % 3 == 0) contrib = e / 3 + 1;
            else contrib = 0;
        } else if (mod == 3 || mod == 5) {
            if (e % 6 == 0) contrib = 1;
            else contrib = 0;
        }
        if (contrib == 0) return 0;
        result *= contrib;
    }

    return result;
}
