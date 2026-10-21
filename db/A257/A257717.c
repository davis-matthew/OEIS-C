/*
 * A257717  a(n) is the smallest odd-indexed prime not included earlier such
 *          that a(n) + a(n-1) + a(n-2) is a prime, beginning with a(1) = 5
 *          and a(2) = 11.
 * https://oeis.org/A257717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257717: a(1)=5, a(2)=11. 
   For n≥3, a(n) is the smallest odd‑indexed prime not used earlier
   such that a(n)+a(n-1)+a(n-2) is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

static seq_t   *seq      = NULL;          /* sequence values */
static idx_t   seq_len   = 0;             /* number of values computed */
static idx_t   seq_cap   = 0;             /* capacity of seq       */

static uint64_t *prime_arr = NULL;       /* array of primes (uint64_t) */
static size_t   prime_cnt   = 0;          /* number of primes built  */
static uint64_t prime_limit = 0;          /* upper bound of primes built */
static uint8_t  *used   = NULL;          /* used[prime_index] flag   */

/* Ensure seq has room for at least 'need' elements */
static void ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if ((idx_t)new_cap < need) new_cap = need;
    seq_t *tmp = seq ? seq_realloc(seq, seq_cap, new_cap) : seq_alloc(new_cap);
    seq = tmp;
    seq_cap = new_cap;
}

/* Extend the prime list to cover at least new_limit, 
   and extend the used array accordingly. */
static void extend_primes(uint64_t new_limit)
{
    uint64_t *tmp;
    size_t   new_cnt;
    build_primes_upto(new_limit, &tmp, &new_cnt);
    free(prime_arr);
    prime_arr = tmp;

    /* Allocate new used array and copy old flags */
    uint8_t *new_used = (uint8_t*)malloc(new_cnt * sizeof(uint8_t));
    for (size_t i = 0; i < prime_cnt; ++i) new_used[i] = used[i];
    free(used);
    used = new_used;

    prime_cnt   = new_cnt;
    prime_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is defined for n ≥ 1 */

    /* First‑time initialisation */
    if (seq == NULL) {
        ensure_seq_cap(16);
        seq[0] = (seq_t)5;          /* a(1) */
        seq[1] = (seq_t)11;         /* a(2) */
        seq_len = 2;

        prime_limit = 1000;
        build_primes_upto(prime_limit, &prime_arr, &prime_cnt);

        used = (uint8_t*)malloc(prime_cnt * sizeof(uint8_t));
        for (size_t i = 0; i < prime_cnt; ++i) used[i] = 0;

        /* Mark primes used in a(1)=5 and a(2)=11 */
        /* 5 is the 3rd prime (index 2), 11 is the 5th prime (index 4) */
        if (prime_cnt > 4) {
            used[2] = 1;  /* prime 5   */
            used[4] = 1;  /* prime 11  */
        }
    }

    /* Compute terms up to requested index n */
    while (seq_len < n) {
        size_t i = 0;  /* iterate over prime indices (0‑based) */
        while (1) {
            /* Grow prime list if needed */
            if (i >= prime_cnt) {
                uint64_t new_lim = prime_limit ? prime_limit << 1 : 1000;
                extend_primes(new_lim);
            }

            /* Consider only odd‑indexed primes (OEIS index = i+1 odd) */
            if ( ((i + 1) & 1) && !used[i] ) {
                seq_t sum = seq[seq_len-1] + seq[seq_len-2] + (seq_t)prime_arr[i];
                if (is_prime_big(sum)) {
                    ensure_seq_cap(seq_len + 1);
                    seq[seq_len] = (seq_t)prime_arr[i];
                    seq_len++;
                    used[i] = 1;
                    break;  /* found a(n) */
                }
            }
            i++;
        }
    }

    return seq[n-1];
}
