/*
 * A249947  Number of available orbitals at increasing subshells in
 *          multi-electron atoms.
 * https://oeis.org/A249947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of available orbitals at increasing subshells
 * following the Aufbau principle.
 *
 * Subshells are enumerated by ascending (n + l) where n is the
 * principal quantum number, l is the azimuthal quantum number
 * (s=0, p=1, d=2, f=3). For equal (n + l) the smaller n (higher
 * l) appears first. Only orbitals with l ≤ 3 and l ≤ n‑1 are
 * considered.  Each orbitals contributes 2*l + 1 orbitals
 * (s=1, p=3, d=5, f=7).  This reproduces sequence A249947.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset 1 – sequence starts at a(1) */
    idx_t count = 0;

    for (idx_t S = 1; ; ++S)
    {
        /* Compute the smallest allowed n for this S:
         *   n ≥ max(1, S-3)   (ensures l ≤ 3)
         *   n ≥ ceil((S+1)/2) (ensures l ≤ n-1)
         */
        idx_t start = S >= 3 ? S - 3 : 1;
        idx_t alt   = (S + 2) / 2;   /* ceil((S+1)/2) */
        if (start < alt) start = alt;

        for (idx_t p = start; p <= S; ++p)
        {
            idx_t l   = S - p;                    /* l = n + l – n */
            seq_t val = (seq_t)(2 * l + 1);       /* degeneracy of the subshell */
            ++count;
            if (count == n) return val;
        }
    }
}
