/*
 * A313879  Coordination sequence Gal.6.343.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for n ≥ 1: a(n) = 5*n + 4*⌊n/12⌋ + μ_{n mod12}(3) +
 *            μ_{n mod12}(6) + μ_{n mod12}(7) + μ_{n mod12}(10), where μ_k(m)
 *            = 1 if m ≤ k else 0
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the extra “+6” steps that occur at indices n mod 12 ∈ {3,6,7,10}. */

seq_t a(idx_t n)
{
    /* Special case for the offset 0 */
    if (n == 0) return (seq_t)1;

    /* Count the number of 6‑increments up to n. */
    idx_t full_cycles = n / 12;            /* number of complete 12‑step blocks */
    seq_t count = (seq_t)full_cycles * 4;   /* each cycle contributes 4 extra steps */

    idx_t rem = n % 12;                    /* remainder of the current block */
    /* Add the extra steps that occur in the partial block.
       The indices with a 6‑increment are 3, 6, 7, 10. */
    if (rem >= 3) count++;
    if (rem >= 6) count++;
    if (rem >= 7) count++;
    if (rem >=10) count++;

    /* Final formula: a(n) = 5*n + count */
    return ((seq_t)5 * n) + count;
}
