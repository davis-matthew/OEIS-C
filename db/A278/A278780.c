/*
 * A278780  Numbers m such that A000041(m) is of the form 2^3 * k for odd k.
 * https://oeis.org/A278780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by searching m in increasing order and keeping those
   for which the partition number p(m) has exactly three factors of 2,
   i.e., p(m) ≡ 8 (mod 16). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *a_vals = NULL;          /* array of sequence terms a(k) */
static size_t a_cap = 0;              /* allocated capacity of a_vals   */
static size_t a_len = 0;              /* number of terms computed so far */

static uint8_t *p_mod = NULL;         /* array of p(m) modulo 16        */
static size_t p_cap = 0;              /* capacity of p_mod              */
static idx_t last_m = 0;              /* largest m for which p_mod has been computed */

/* extend the sequence up to at least 'target' terms */
static void compute_terms(idx_t target)
{
    while (a_len < target) {
        /* compute partition modulo 16 for next m */
        ++last_m;

        /* ensure storage for p_mod[last_m] */
        if (p_cap <= (size_t)last_m) {
            p_cap = p_cap ? p_cap * 2 : 16;
            uint8_t *tmp = (uint8_t *)realloc(p_mod, p_cap * sizeof(uint8_t));
            p_mod = tmp;
        }

        int sum = 0;
        for (unsigned int k = 1; ; ++k) {
            unsigned int g1 = (k * (3 * k - 1)) >> 1;   /* k(3k-1)/2 */
            if (g1 > last_m) break;
            int sign = (k & 1) ? 1 : -1;
            sum += sign * p_mod[(size_t)(last_m - g1)];
            unsigned int g2 = (k * (3 * k + 1)) >> 1;   /* k(3k+1)/2 */
            if (g2 > last_m) continue;
            sum += sign * p_mod[(size_t)(last_m - g2)];
        }

        int val = sum % 16;           /* modulo 16, keep positive */
        if (val < 0) val += 16;
        p_mod[(size_t)last_m] = (uint8_t)val;

        /* if v₂(p(m)) == 3, record m */
        if (val == 8) {
            if (a_cap <= a_len) {
                a_cap = a_cap ? a_cap * 2 : 16;
                seq_t *tmp2 = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
                a_vals = tmp2;
            }
            a_vals[a_len++] = (seq_t)last_m;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n = 0 */

    /* initialisation of p_mod */
    if (a_vals == NULL) {
        p_cap = 16;
        p_mod = (uint8_t *)realloc(NULL, p_cap * sizeof(uint8_t));
        p_mod[0] = 1;                     /* p(0) = 1 */
        last_m = 0;
    }

    if (a_len < n) compute_terms(n);
    return a_vals[n - 1];
}
