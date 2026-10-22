/*
 * A342175  a(n) is the difference between the n-th composite number and the
 *          smallest larger composite to which it is relatively prime.
 * https://oeis.org/A342175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This routine implements a(n) = A113496(n) – A002808(n): For the
 *            n‑th composite number c it finds the smallest larger composite d
 *            coprime to c and returns d−c
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

static seq_t *differences = NULL;   /* a(1), a(2), … */
static size_t capacity      = 0;    /* allocated size of differences    */
static size_t terms_computed = 0;   /* number of terms already computed */
static idx_t last_comp_number = 3;  /* last composite processed (4 is first) */

/* Ensure that the first n terms are computed. */
static void ensure_terms(idx_t n)
{
    if (n <= terms_computed) return;

    /* Resize array if necessary. */
    if (n > capacity) {
        size_t newcap = capacity ? capacity * 2 : 1024;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(differences, newcap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        differences = tmp;
        capacity    = newcap;
    }

    /* Find the next composite number after the last processed one. */
    idx_t cur = last_comp_number + 1;

    while (terms_computed < n) {
        /* Advance to the next composite number. */
        while (is_prime(cur)) ++cur;

        seq_t c = (seq_t)cur;           /* current composite as seq_t */

        /* Search for the smallest larger composite coprime to c. */
        idx_t m = cur + 1;
        for (;;) {
            if (!is_prime(m) && gcd(c, (seq_t)m) == (seq_t)1) {
                differences[terms_computed] = (seq_t)(m - cur);
                break;
            }
            ++m;
        }

        /* Prepare for the next iteration. */
        ++terms_computed;
        last_comp_number = cur;
        ++cur;                        /* move past the composite just processed */
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */
    ensure_terms(n);
    return differences[n - 1];          /* a(1) stored at differences[0] */
}
