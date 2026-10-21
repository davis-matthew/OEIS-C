/*
 * A283636  Number of 3Xn 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            6*a(n-1)+9*a(n-2)-43*a(n-3)-213*a(n-4)+69*a(n-5)+946*a(n-6)
 *            +1929*a(n-7)+1017*a(n-8)+2204*a(n-9)+1875*a(n-10)+2487*a(n-11)
 *            -2529*a(n-12)+5073*a(n-13)-3762*a(n-14)+2255*a(n-15)-798*a(n-16)
 *            +132*a(n-17)-8*a(n-18) (recurrence of order 18) Initial values
 *            (a(1) … a(18)): 0, 6, 37, 639, 2875, 21...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  initial terms a(1)..a(18)  */
    static const uint64_t init_vals[18] = {
        0ULL,          /* a(1) */
        6ULL,          /* a(2) */
        37ULL,         /* a(3) */
        639ULL,        /* a(4) */
        2875ULL,       /* a(5) */
        21963ULL,      /* a(6) */
        127635ULL,     /* a(7) */
        693783ULL,     /* a(8) */
        3852792ULL,    /* a(9) */
        20064881ULL,   /* a(10) */
        103580688ULL,  /* a(11) */
        524492026ULL,  /* a(12) */
        2611146243ULL, /* a(13) */
        12857829918ULL,/* a(14) */
        62584105607ULL,/* a(15) */
        301823257243ULL,/* a(16) */
        1443755263689ULL,/* a(17) */
        6855724802293ULL /* a(18) */
    };

    /* coefficients for the recurrence */
    static const long long posCoefs[12] = {6, 9, 69, 946, 1929, 1017, 2204,
                                            1875, 2487, 5073, 2255, 132};
    static const int posIdx[12] = {1, 2, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17};

    static const long long negCoefs[6] = {43, 213, 2529, 3762, 798, 8};
    static const int negIdx[6] = {3, 4, 12, 14, 16, 18};

    if (n == 0)                /* undefined (offset 1), return 0 for safety */
        return (seq_t)0;

    /* static cache for memoization */
    static seq_t *table   = NULL;
    static idx_t table_len = 0;   /* number of terms actually stored */
    static idx_t table_cap = 0;   /* allocated capacity */

    /* initial allocation & pre‑load of a(1)..a(18) */
    if (table == NULL) {
        idx_t cap = (n > 18) ? n : 18;
        table = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        for (idx_t i = 0; i < 18; ++i)
            table[i] = (seq_t)init_vals[i];
        table_len = 18;
        table_cap = cap;
    }
    /* grow array if needed */
    else if (n > table_cap) {
        idx_t newcap = table_cap * 2;
        if (newcap < n) newcap = n;
        table = (seq_t *)realloc(table, (size_t)newcap * sizeof(seq_t));
        table_cap = newcap;
    }

    /* compute missing terms up to n */
    if (n > table_len) {
        for (idx_t i = table_len; i < n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            /* positive part */
            for (int k = 0; k < 12; ++k)
                pos += (seq_t)posCoefs[k] * table[i - posIdx[k]];
            /* negative part */
            for (int k = 0; k < 6; ++k)
                neg += (seq_t)negCoefs[k] * table[i - negIdx[k]];
            table[i] = pos - neg;
        }
        table_len = n;
    }

    return table[n - 1];   /* a(1) is stored at index 0 */
}
