/*
 * A244005  Atomic numbers of the pre-transition metals.
 * https://oeis.org/A244005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the atomic number of the n-th pre‑transition metal *
 *            (group 1, group 2, and the single group 13 element Al). The
 *            sequence * is defined for 1 ≤ n ≤ 13 as shown in the OEIS entry.
 *            For n > 13 the * value is undefined; the function returns 0
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t t[] = {
        0ULL, /* dummy 0 for 0‑index */
        3ULL, /* Li   */
        4ULL, /* Be   */
        11ULL,/* Na   */
        12ULL,/* Mg   */
        13ULL,/* Al   */
        19ULL,/* K    */
        20ULL,/* Ca   */
        37ULL,/* Rb   */
        38ULL,/* Sr   */
        55ULL,/* Cs   */
        56ULL,/* Ba   */
        87ULL,/* Fr   */
        88ULL  /* Ra   */
    };

    if (n == 0u)
        return 0;                /* below offset – undefined behaviour */
    if (n <= 13u)
        return t[n];
    return 0;                    /* beyond known terms – undefined */
}
