/*
 * A244318  Product of digits+1 of n in Greedy Catalan Base (A014418).
 * https://oeis.org/A244318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏(d_k+1) for digits d_k in the greedy Catalan‑base
 *            representation of n
 * Range:     u8 n <= 388, u16 n <= 4862, u32 n <= 4862, u64 n <= 4862,
 *            u128 n <= 4862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache of Catalan numbers – start with C0 = 1 */
static seq_t *Catalan = NULL;
static size_t Catalan_len = 0;
static size_t Catalan_cap = 0;

/* generate Catalan numbers while the last one ≤ n */
static void ensure_catalan_upto(idx_t n) {
    if (Catalan_len == 0) {
        Catalan_cap = 4;
        Catalan = (seq_t *)malloc(Catalan_cap * sizeof(seq_t));
        Catalan[0] = (seq_t)1;          /* C0 = 1 */
        Catalan_len = 1;
    }
    /* produce next values using the recurrence
       C_(k+1) = C_k * 2 * (2k+1) / (k+2)     */
    while ((idx_t)Catalan[Catalan_len - 1] <= n) {
        size_t k = Catalan_len - 1;           /* index of last stored value */
        /* use 128‑bit arithmetic to avoid overflow in 64‑bit builds */
        unsigned __int128 cur  = (unsigned __int128)Catalan[k];
        unsigned __int128 num  = cur * (unsigned __int128)2 *
                                 ((unsigned __int128)2 * (unsigned __int128)k + (unsigned __int128)1);
        unsigned __int128 next128 = num / ((unsigned __int128)k + (unsigned __int128)2);
        if (next128 > (unsigned __int128)n)   /* we only need values ≤ n */
            break;
        if (Catalan_len == Catalan_cap) {
            Catalan_cap <<= 1;
            Catalan = (seq_t *)realloc(Catalan, Catalan_cap * sizeof(seq_t));
        }
        Catalan[Catalan_len++] = (seq_t)next128;
    }
}

seq_t a(idx_t n) {
    seq_t prod = (seq_t)1;
    if (n == 0)
        return prod;

    ensure_catalan_upto(n);

    /* greedy decomposition – start with the largest admissible Catalan number */
    for (size_t i = Catalan_len; i-- > 0; ) {
        seq_t cat = Catalan[i];
        idx_t cat64 = (idx_t)cat;          /* cat ≤ n, safe cast */
        if (cat64 > n)
            continue;
        idx_t coeff = n / cat64;
        if (coeff) {
            prod *= (seq_t)(coeff + 1);
            n -= coeff * cat64;
            if (n == 0)
                break;
        }
    }
    return prod;
}
