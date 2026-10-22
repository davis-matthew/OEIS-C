/*
 * A375159  Expansion of Sum_{k in Z} x^(4*k) / (1 - x^(7*k+2)).
 * https://oeis.org/A375159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = #{k ≥ 0 | n ≡ 4k (mod 7k+2)} - #{t ≥ 1 | n ≡ 3t-2 (mod
 *            7t-2)}
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count solutions of n = 4k + m(7k+2) for k≥0, m≥0, and
 * subtract solutions of n = 3t-2 + m(7t-2) for t≥1, m≥0.  */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate products to avoid overflow. */
    unsigned __int128 un = n;

    idx_t pos = 0;   /* Solutions with k ≥ 0. */
    for (idx_t k = 0;; ++k) {
        unsigned __int128 fourk = (unsigned __int128)4 * k;
        if (fourk > un) break;               // 4k > n  ⇒ no further non‑negative solutions.

        unsigned __int128 denom = (unsigned __int128)7 * k + 2;   // 7k+2
        unsigned __int128 rem   = un - fourk;                    // n – 4k

        if ((rem % denom) == 0) ++pos;          // m = (n‑4k)/(7k+2) is an integer ≥ 0
    }

    idx_t neg = 0;   /* Solutions with k < 0, rewritten using t = –k ≥ 1. */
    for (idx_t t = 1;; ++t) {
        unsigned __int128 base = (unsigned __int128)3 * t - 2;    // 3t–2
        if (base > un) break;                                  // 3t–2 > n  ⇒ stop

        unsigned __int128 denom = (unsigned __int128)7 * t - 2;   // 7t–2
        unsigned __int128 rem   = un - base;                    // n – (3t–2)

        if ((rem % denom) == 0) ++neg;          // m = (n‑3t+2)/(7t‑2) is an integer ≥ 0
    }

    /* The final coefficient is the signed difference. */
    long long diff = (long long)pos - (long long)neg;
    return (seq_t)diff;
}
