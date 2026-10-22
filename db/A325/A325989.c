/*
 * A325989  Number of perfect factorizations of n.
 * https://oeis.org/A325989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of perfect factorizations of n. The implementation
 *            precomputes the number of perfect factorizations for every prime
 *            power p^e (e up to 64). For a composite n, a(n) is the product
 *            of the values for its prime‑power factors. The computation of
 *            the prime‑power count enumerates all parti...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   enum_parts, valid_parts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/enum_parts.h"
#include "../utils/valid_parts.h"

/* --- precomputation of f[e] : number of perfect factorizations of p^e --- */

static seq_t *f_table = NULL;          /* table f[e] */
static int   f_max_exp = 64;           /* maximum exponent covered */

/* function prototypes */
static void   prepare_f(void);
static int    valid_parts(const int *parts, int depth, int e);
static void   enum_parts(int remaining, int min_part,
                         int depth, int *parts,
                         seq_t *cnt, int e);

/* create f_table if not yet prepared */
static void prepare_f(void) {
    if (f_table != NULL) return;
    f_table = (seq_t*)malloc((f_max_exp + 1) * sizeof(seq_t));
    memset(f_table, 0, (f_max_exp + 1) * sizeof(seq_t));
    f_table[0] = (seq_t)1;              /* not used, but harmless */

    int parts[64];
    for (int e = 1; e <= f_max_exp; ++e) {
        seq_t cnt = (seq_t)0;
        enum_parts(e, 1, 0, parts, &cnt, e);
        f_table[e] = cnt;
    }
}

/* --- public sequence function --- */

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;    /* a(1) = 1 */

    prepare_f();                            /* precompute f_table */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = (idx_t)2;
         p * p <= temp;  /* safe for n <= 10^9, and faster than sqrt() */
         ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result = result * f_table[exp];
        }
    }
    if (temp > (idx_t)1) {
        /* remaining prime factor with exponent 1 */
        result = result * f_table[1];
    }

    return result;
}
