/*
 * A287524  Start with 0 and repeatedly substitute 0->012, 1->102, 2->201.
 * https://oeis.org/A287524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(3)=2; for n>3 let p=ceil(n/3) and r=(n-1)%3.
 *            Then a(n)=M[ a(p) ][ r ] where M is the morphism table 0->012,
 *            1->102, 2->201
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _base[3] = {0, 1, 2};
static const seq_t _morph[3][3] = {
    {0, 1, 2},   /* 0 -> 012 */
    {1, 0, 2},   /* 1 -> 102 */
    {2, 0, 1}    /* 2 -> 201 */
};

seq_t a(idx_t n)
{
    if (n <= 3)
        return _base[n - 1];
    idx_t p = (n - 1) / 3 + 1;          /* parent index */
    unsigned r = (n - 1) % 3;           /* position within the morphism block */
    seq_t parent = a(p);                /* recursively compute parent digit */
    return _morph[(unsigned)parent][r];
}
