/*
 * A351391  Maximum k for which the grid graph P_2 X P_k is an induced
 *          subgraph of the n X n knight graph.
 * https://oeis.org/A351391
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n^2 - p(n)) / 6, n ≥ 3. * Here p(n) depends on n mod 6:
 *            * n mod 6 = 0 → p(n) = 2 n – 24 * n mod 6 = 1 → p(n) = 1 * n mod
 *            6 = 2 → p(n) = 4 * n mod 6 = 3 → p(n) = 9 * n mod 6 = 4 → p(n) =
 *            n * n mod 6 = 5 → p(n) = n – 4 * Special cases: a(3)=1, a(5)=5.
 *            The formula is valid for all n≥4
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)1;
    if (n == 5) return (seq_t)5;

    seq_t n2 = (seq_t)n * n;
    idx_t mod = n % 6;

    if (mod == 0) {
        /* Use n^2 – (2 n – 24) = n^2 – 2 n + 24 to avoid negative values. */
        return (n2 - (seq_t)2 * n + 24) / 6;
    } else {
        seq_t p;
        switch (mod) {
            case 1: p = 1;   break;
            case 2: p = 4;   break;
            case 3: p = 9;   break;
            case 4: p = (seq_t)n;           break;
            case 5: p = (seq_t)n - 4;       break;
            default: p = 0; /* unreachable */ break;
        }
        return (n2 - p) / 6;
    }
}
