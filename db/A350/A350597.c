/*
 * A350597  Irregular triangle read by rows in which row n lists the partial
 *          sums of shell numbers and respective number of electrons for all
 *          occupied shells of the n-th element of the periodic table of the
 *          elements where electron configurations are ordered according to
 *          Madelung's rule.
 * https://oeis.org/A350597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derivation: a(n) is the nth element of the irregular triangle of
 * electron configuration partial sums, constructed by the
 * Madelung rule.  For each element, its occupied shells are
 * listed in order of increasing (n + l), with tie broken by n.
 * For each occupied shell (principal quantum number n, orbital
 * stoichiometry l) a pair of numbers is produced: n followed by the
 * number of electrons in that shell.  The sequence consists of
 * the cumulative partial sums of these numbers read row by row. */

#include <stddef.h>

typedef struct {int n; int l; int cap;} Orbital;

seq_t a(idx_t n)
{
    /* static initialization of orbital list */
    static int init = 0;
    static Orbital orbitals[200];
    static size_t orbitals_cnt = 0;

    if (!init) { /* generate and sort orbitals only once */
        size_t c = 0;
        for (int N = 1; N <= 20 && c < 200; ++N) {      /* N: principal quantum number */
            for (int L = 0; L < N && c < 200; ++L) { /* L: azimuthal quantum number */
                orbitals[c].n   = N;
                orbitals[c].l   = L;
                orbitals[c].cap = 2 * (2 * L + 1);      /* maximum electrons in this sub-shell */
                ++c;
            }
        }
        orbitals_cnt = c;
        /* sort orbitals by (n + l) ascending, then n ascending */
        for (size_t i = 0; i + 1 < orbitals_cnt; ++i)
            for (size_t j = 0; j + 1 < orbitals_cnt - i - 1; ++j) {
                int key1 = orbitals[j].n + orbitals[j].l;
                int key2 = orbitals[j + 1].n + orbitals[j + 1].l;
                if (key1 > key2 ||
                    (key1 == key2 && orbitals[j].n > orbitals[j + 1].n)) {
                    Orbital tmp = orbitals[j];
                    orbitals[j] = orbitals[j + 1];
                    orbitals[j + 1] = tmp;
                }
            }
        init = 1;
    }

    idx_t term_idx = 0;          /* count of terms already produced */

    for (idx_t atomic = 1; ; ++atomic) {   /* atomic number of the element */
        idx_t remain = atomic;            /* electrons left to fill */
        seq_t  sum   = 0;                 /* local partial sum for this row */

        for (size_t oi = 0; oi < orbitals_cnt && remain > 0; ++oi) {
            size_t fill = remain < (idx_t)orbitals[oi].cap ? remain
                                                            : orbitals[oi].cap;

            /* add shell principal quantum number */
            sum += orbitals[oi].n;
            ++term_idx;
            if (term_idx == n) return sum;

            /* add electrons in this shell */
            sum += (seq_t)fill;
            ++term_idx;
            if (term_idx == n) return sum;

            remain -= fill;
        }
        /* continue with next element */
    }
    /* unreachable but required to satisfy compiler */
    return 0;
}
