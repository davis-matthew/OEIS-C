/*
 * A259996  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000011 or 00000101.
 * https://oeis.org/A259996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical linear recurrence for a(n) (n > 22):
   a(n) = 12*a(n-3) + 22*a(n-4) - 31*a(n-6) - 66*a(n-7)
        + 42*a(n-8) + 22*a(n-9) + 114*a(n-10) - 169*a(n-11)
        + 417*a(n-12) - 218*a(n-13) + 439*a(n-14) - 585*a(n-15)
        - 36*a(n-16) - 74*a(n-17) + 54*a(n-18) + 90*a(n-19).
   Initial values for 1 ≤ n ≤ 22 are listed below. */

#include <stddef.h>
#include <stdlib.h>

/* initial segment, index 0 unused */
static const seq_t init[23] = {
    0ULL, /* unused */
    (seq_t)284ULL, (seq_t)805ULL, (seq_t)2532ULL, (seq_t)6157ULL,
    (seq_t)13562ULL, (seq_t)39209ULL, (seq_t)112570ULL,
    (seq_t)265595ULL, (seq_t)662116ULL, (seq_t)1906817ULL,
    (seq_t)4983158ULL, (seq_t)12082675ULL, (seq_t)32339598ULL,
    (seq_t)88603673ULL, (seq_t)223563456ULL, (seq_t)569090083ULL,
    (seq_t)1539186674ULL, (seq_t)4049079097ULL, (seq_t)10269959842ULL,
    (seq_t)26926828037ULL, (seq_t)71786606702ULL, (seq_t)185590064541ULL
};

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* below offset – not used */

    static seq_t *seq = NULL;          /* 1‑based storage */
    static size_t cap   = 0;           /* allocated capacity (#elements) */
    static idx_t  max_comp = 0;       /* highest computed index */

    /* Allocate or grow storage */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 64;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise initial segment if not yet done */
    if (max_comp < 22) {
        for (idx_t i = max_comp + 1; i <= 22; ++i)
            seq[i] = init[i];
        max_comp = 22;
    }

    /* Return if already computed */
    if (n <= max_comp)
        return seq[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t s = 0;
        s += (seq_t)12  * seq[i-3];
        s += (seq_t)22  * seq[i-4];
        s -= (seq_t)31  * seq[i-6];
        s -= (seq_t)66  * seq[i-7];
        s += (seq_t)42  * seq[i-8];
        s += (seq_t)22  * seq[i-9];
        s += (seq_t)114 * seq[i-10];
        s -= (seq_t)169 * seq[i-11];
        s += (seq_t)417 * seq[i-12];
        s -= (seq_t)218 * seq[i-13];
        s += (seq_t)439 * seq[i-14];
        s -= (seq_t)585 * seq[i-15];
        s -= (seq_t)36  * seq[i-16];
        s -= (seq_t)74  * seq[i-17];
        s += (seq_t)54  * seq[i-18];
        s += (seq_t)90  * seq[i-19];
        seq[i] = s;
    }

    max_comp = n;
    return seq[n];
}
