/*
 * A246743  Number of length 6+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 3*a(n-1) +5*a(n-2) -23*a(n-3) -4*a(n-4) +76*a(n-5) -28*a(n-6)
          -140*a(n-7) +98*a(n-8)+154*a(n-9) -154*a(n-10) -98*a(n-11)
          +140*a(n-12)+28*a(n-13)-76*a(n-14)+4*a(n-15)+23*a(n-16)
          -5*a(n-17)-3*a(n-18)+a(n-19)  for n>19
   Initial values (n from 1 to 19):
     2, 52, 5132, 52112, 807630, 4791012, 31944440, 130526848,
     540366650, 1692606260, 5207709252, 13469585232, 33976308422,
     76317563812, 167224561520, 337577831552, 666340346610,
     1237996390068, 2255112904700 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, safe return */

    /* ----- Static cache for n <= 10000 ----- */
    static seq_t static_cache[10001] = {0};
    static idx_t static_computed = 0;

    /* ----- Dynamic cache for n > 10000 ----- */
    static seq_t *dyn_cache = NULL;
    static idx_t dyn_size = 0;
    static idx_t dyn_computed = 0;

    /* Recurrence coefficients (order 19) */
    static const int coeffs[19] = {
        3, 5, -23, -4, 76, -28, -140, 98, 154, -154,
        -98, 140, 28, -76, 4, 23, -5, -3, 1
    };

    /* ----- Initialize static cache on first use ----- */
    if (static_computed == 0) {
        static_cache[1]  = (seq_t)2;
        static_cache[2]  = (seq_t)52;
        static_cache[3]  = (seq_t)5132;
        static_cache[4]  = (seq_t)52112;
        static_cache[5]  = (seq_t)807630;
        static_cache[6]  = (seq_t)4791012;
        static_cache[7]  = (seq_t)31944440;
        static_cache[8]  = (seq_t)130526848;
        static_cache[9]  = (seq_t)540366650;
        static_cache[10] = (seq_t)1692606260;
        static_cache[11] = (seq_t)5207709252;
        static_cache[12] = (seq_t)13469585232;
        static_cache[13] = (seq_t)33976308422;
        static_cache[14] = (seq_t)76317563812;
        static_cache[15] = (seq_t)167224561520;
        static_cache[16] = (seq_t)337577831552;
        static_cache[17] = (seq_t)666340346610;
        static_cache[18] = (seq_t)1237996390068;
        static_cache[19] = (seq_t)2255112904700;
        static_computed = 19;
    }

    /* ----- Handle requests that fit in the static table ----- */
    if (n <= 10000) {
        if (n <= static_computed) {
            return static_cache[n];
        }
        /* Compute missing values up to n */
        for (idx_t i = static_computed + 1; i <= n; ++i) {
            if (i <= 19) continue;            /* already initialized */
            seq_t sum = 0;
            for (int k = 0; k < 19; ++k) {
                sum += (seq_t)coeffs[k] * static_cache[i - (k + 1)];
            }
            static_cache[i] = sum;
        }
        static_computed = n;
        return static_cache[n];
    }

    /* ----- Initialize dynamic cache on first use for n > 10000 ----- */
    if (dyn_cache == NULL) {
        idx_t init_size = n + 1;
        if (init_size < 20) init_size = 20;
        dyn_cache = (seq_t *)malloc(sizeof(seq_t) * init_size);
        if (!dyn_cache) return 0; /* allocation failure */

        /* Zero all entries */
        for (idx_t i = 0; i < init_size; ++i) dyn_cache[i] = 0;

        /* Initialize first 19 terms */
        dyn_cache[1]  = (seq_t)2;
        dyn_cache[2]  = (seq_t)52;
        dyn_cache[3]  = (seq_t)5132;
        dyn_cache[4]  = (seq_t)52112;
        dyn_cache[5]  = (seq_t)807630;
        dyn_cache[6]  = (seq_t)4791012;
        dyn_cache[7]  = (seq_t)31944440;
        dyn_cache[8]  = (seq_t)130526848;
        dyn_cache[9]  = (seq_t)540366650;
        dyn_cache[10] = (seq_t)1692606260;
        dyn_cache[11] = (seq_t)5207709252;
        dyn_cache[12] = (seq_t)13469585232;
        dyn_cache[13] = (seq_t)33976308422;
        dyn_cache[14] = (seq_t)76317563812;
        dyn_cache[15] = (seq_t)167224561520;
        dyn_cache[16] = (seq_t)337577831552;
        dyn_cache[17] = (seq_t)666340346610;
        dyn_cache[18] = (seq_t)1237996390068;
        dyn_cache[19] = (seq_t)2255112904700;

        dyn_size = init_size;
        dyn_computed = 19;
    }

    if (n <= dyn_computed) {
        return dyn_cache[n];
    }

    /* Ensure capacity */
    if (n + 1 > dyn_size) {
        idx_t new_size = n + 1;
        if (new_size < dyn_size * 2) new_size = dyn_size * 2;
        seq_t *tmp = (seq_t *)realloc(dyn_cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0; /* allocation failure */
        /* Zero newly allocated part */
        for (idx_t i = dyn_size; i < new_size; ++i) tmp[i] = 0;
        dyn_cache = tmp;
        dyn_size = new_size;
    }

    /* Compute new terms up to n */
    for (idx_t i = dyn_computed + 1; i <= n; ++i) {
        if (i <= 19) continue;   /* already initialized */
        seq_t sum = 0;
        for (int k = 0; k < 19; ++k) {
            sum += (seq_t)coeffs[k] * dyn_cache[i - (k + 1)];
        }
        dyn_cache[i] = sum;
    }
    dyn_computed = n;
    return dyn_cache[n];
}
