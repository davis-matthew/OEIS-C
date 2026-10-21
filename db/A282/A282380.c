/*
 * A282380  Number of ways to write n as a sum of two unordered nonsquarefree
 *          numbers.
 * https://oeis.org/A282380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of nonsquarefree integers whose
 *            sum equals n. A nonsquarefree integer is divisible by p² for
 *            some prime p
 * Range:     u8 n <= 1547, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for the sieve */
static uint8_t   *is_nsf   = NULL;   /* is_nsf[i] == 1 iff i is nonsquarefree */
static size_t    nsf_cap  = 0;       /* allocated capacity of is_nsf          */
static idx_t    *nsf_list = NULL;   /* ascending list of nonsquarefree ints  */
static size_t    nsf_len  = 0;       /* number of elements in nsf_list        */

/*  Ensure that the sieve data covers up to n (inclusive).  */
static void
ensure_nsf(idx_t n)
{
    if (nsf_cap > 0 && (size_t)n < nsf_cap) return;   /* already computed */

    /* Allocate fresh arrays */
    size_t cap = (size_t)n + 1;
    uint8_t *new_is_nsf = (uint8_t*)calloc(cap, 1);
    if (new_is_nsf == NULL) return;   /* OOM – caller will see 0 */

    /* Simple sieve of Eratosthenes up to n */
    uint8_t *is_prime = (uint8_t*)malloc(cap);
    if (is_prime == NULL) {
        free(new_is_nsf);
        return;
    }
    for (size_t i = 0; i < cap; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p <= n / p; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p) {
                is_prime[m] = 0;
            }
        }
    }

    /* Mark nonsquarefree numbers (multiples of p²) */
    for (idx_t p = 2; p <= n / p; ++p) {
        if (is_prime[p]) {
            idx_t p2 = p * p;
            for (idx_t m = p2; m <= n; m += p2) {
                new_is_nsf[m] = 1;
            }
        }
    }
    free(is_prime);

    /* Build list of nonsquarefree numbers */
    idx_t *new_list = (idx_t*)malloc(cap * sizeof(idx_t));
    if (new_list == NULL) {
        free(new_is_nsf);
        return;
    }
    size_t len = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (new_is_nsf[i]) {
            new_list[len++] = i;
        }
    }

    /* Replace old data */
    free(is_nsf);
    free(nsf_list);
    is_nsf   = new_is_nsf;
    nsf_list = new_list;
    nsf_cap  = cap;
    nsf_len  = len;
}

/* Main function ----------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 2) return 0;         /* no unordered pair exists */

    ensure_nsf(n);
    if (is_nsf == NULL) return 0;   /* OOM guard */

    seq_t count = 0;
    idx_t half = n / 2;
    for (size_t idx = 0; idx < nsf_len; ++idx) {
        idx_t a_val = nsf_list[idx];
        if (a_val > half) break;
        idx_t b_val = n - a_val;
        if (b_val >= 2 && is_nsf[b_val]) ++count;
    }
    return count;
}
