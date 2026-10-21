/*
 * A287889  Number of rooted labeled 4-cactus graphs on 3n+1 nodes.
 * https://oeis.org/A287889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)^n * (3*n)! / (2^n * n!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any nonnegative n.  Uses only integer arithmetic */
seq_t a(idx_t n)
{
    /* Result accumulator */
    seq_t result = 1;
    /* Number of 2-factors still needed to divide out in the denominator */
    idx_t rem = n;

    /* Base of the power term */
    seq_t base = 3*n + 1;

    /* Multiply (base)^n , canceling 2-factors as we go */
    for (idx_t i = 0; i < n; ++i) {
        seq_t t = base;
        /* Remove as many factors of 2 from the current multiplier as possible */
        while (rem > 0 && (t % 2 == 0)) {
            t /= 2;
            --rem;
        }
        result *= t;
    }

    /* Multiply the remaining factorial factor product (n+1) ... (3n) */
    for (idx_t k = n + 1; k <= 3*n; ++k) {
        seq_t t = k;
        /* Remove factors of 2 here as well */
        while (rem > 0 && (t % 2 == 0)) {
            t /= 2;
            --rem;
        }
        result *= t;
    }

    /* Any leftover powers of 2 in the denominator should now be applied */
    for (idx_t i = 0; i < rem; ++i) {
        result /= 2;
    }

    return result;
}
