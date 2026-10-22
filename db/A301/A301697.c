/*
 * A301697  Coordination sequence for node of type V2 in "krj" 2-D tiling (or
 *          net).
 * https://oeis.org/A301697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=10, a(3)=16, a(4)=22, a(5)=27, a(6)=32 a(n)
 *            = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5) - a(n-6)
 *            (n≥7)
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *tab = NULL;
    static idx_t sz = 0;

    if (sz == 0) {
        sz = 7;
        tab = (seq_t*)malloc(sz * sizeof(seq_t));
        if (tab == NULL) abort();
        tab[0] = (seq_t)1;
        tab[1] = (seq_t)5;
        tab[2] = (seq_t)10;
        tab[3] = (seq_t)16;
        tab[4] = (seq_t)22;
        tab[5] = (seq_t)27;
        tab[6] = (seq_t)32;
    }

    if (n < sz) return tab[n];

    seq_t *newtab = (seq_t*)realloc(tab, (n + 1) * sizeof(seq_t));
    if (newtab == NULL) abort();
    tab = newtab;

    for (idx_t i = sz; i <= n; ++i) {
        seq_t t =
            ((seq_t)2 * tab[i-1]) - ((seq_t)2 * tab[i-2]) + ((seq_t)2 * tab[i-3])
          - ((seq_t)2 * tab[i-4]) + ((seq_t)2 * tab[i-5]) - tab[i-6];
        tab[i] = t;
    }
    sz = n + 1;
    return tab[n];
}
