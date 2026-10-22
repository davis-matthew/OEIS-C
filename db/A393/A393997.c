/*
 * A393997  Number of steps to reach the end of the cycle starting with
 *          n/(2n+1), or -1 if the end of the cycle is never reached, in the
 *          following process: If the numerator is even, divide the fraction
 *          by 2, otherwise multiply the fraction by 3 and add 1.
 * https://oeis.org/A393997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps in the generalized Collatz trajectory
 *            starting from n/(2n+1) until the first repeated fraction appears
 * Range:     u8 n <= 8, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

typedef struct { seq_t num; seq_t den; } frac_t;

/* Compute gcd for any seq_t supporting % and <. */
static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Resize an array of frac_t, preserving its contents.  Works for both
   C (using malloc/realloc) and C++ (using new/delete). */
static void grow_array(frac_t **arr, size_t *cap, size_t needed) {
    if (needed <= *cap) return;
    size_t newcap = *cap ? (*cap << 1) : 8;
    if (newcap < needed) newcap = needed;

#ifdef __cplusplus
    frac_t *newarr = new frac_t[newcap];
    if (*arr) {
        for (size_t i = 0; i < *cap; ++i) newarr[i] = (*arr)[i];
        delete[] *arr;
    }
    *arr = newarr;
#else
    *arr = (frac_t*)realloc(*arr, newcap * sizeof(frac_t));
#endif
    *cap = newcap;
}

/* Free an array allocated by grow_array. */
static void free_array(frac_t *arr) {
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t num = (seq_t)n;
    seq_t den = (seq_t)2 * n + 1;
    seq_t count = 0;

    frac_t *tab = NULL;
    size_t cap = 0, len = 0;

    /* Store initial fraction */
    grow_array(&tab, &cap, 1);
    tab[0].num = num;
    tab[0].den = den;
    len = 1;

    while (1) {
        seq_t next_num, next_den;

        if ((num & 1) == 0) {               /* numerator even: divide by 2 */
            next_num = num;
            next_den = den * 2;
        } else {                            /* numerator odd: 3*num + den */
            next_num = 3 * num + den;
            next_den = den;
        }

        /* Reduce fraction */
        seq_t g = seq_gcd(next_num, next_den);
        next_num /= g;
        next_den /= g;

        /* Check for repetition */
        size_t i;
        for (i = 0; i < len; ++i) {
            if (tab[i].num == next_num && tab[i].den == next_den) break;
        }
        if (i < len) break;                 /* repeat detected */

        /* Add new fraction */
        grow_array(&tab, &cap, len + 1);
        tab[len].num = next_num;
        tab[len].den = next_den;
        ++len;
        ++count;

        num = next_num;
        den = next_den;
    }

    free_array(tab);
    return count;
}
