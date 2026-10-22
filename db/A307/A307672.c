/*
 * A307672  The right half of a bi-infinite word invariant under the balanced
 *          morphism, {0->501, 1->210, 2->123, 3->432, 4->345, 5->054},
 *          starting from axiom a(0)=0.
 * https://oeis.org/A307672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0. For n > 0, let i = floor((n+1)/3) and d = (n+1) mod 3.
 *            Then a(n) = morph[a(i)][d], where morph[u][0..2] is the 3‑symbol
 *            image of symbol u under the balanced morphism 0→5 0 1, 1→2 1 0,
 *            2→1 2 3, 3→4 3 2, 4→3 4 5, 5→0 5 4
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t morph[6][3] = {
    {5, 0, 1},   /* 0 → 5 0 1 */
    {2, 1, 0},   /* 1 → 2 1 0 */
    {1, 2, 3},   /* 2 → 1 2 3 */
    {4, 3, 2},   /* 3 → 4 3 2 */
    {3, 4, 5},   /* 4 → 3 4 5 */
    {0, 5, 4}    /* 5 → 0 5 4 */
};

static seq_t _a_recursive(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute (n+1)/3 and (n+1)%3 without overflowing */
    idx_t q = n / 3;
    unsigned r = (unsigned)(n % 3);          /* r ∈ {0,1,2}          */
    idx_t i = (r < 2) ? q : q + 1;           /* parent index          */
    unsigned d = (r + 1) % 3;                /* digit for morph array */

    seq_t parent = _a_recursive(i);
    size_t pi = (size_t)parent;              /* safe: parent ∈ {0..5} */
    return morph[pi][d];
}

seq_t a(idx_t n)
{
    return _a_recursive(n);
}
