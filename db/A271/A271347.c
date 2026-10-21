/*
 * A271347  Primes p such that p + 38 is also prime.
 * https://oeis.org/A271347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 920, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A271347: primes p such that p + 38 is also prime.
   Indexed from 1.  The function builds a sieve once and caches the
   needed primes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *seq = NULL;         /* cached primes satisfying the condition */
static size_t seq_len = 0;        /* number of stored primes */
static size_t seq_cap = 0;        /* allocated capacity */
static bool initialized = false;

/* enlarge the array for storing sequence elements */
static void push_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* allocation failed – data lost */
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* build the sequence once, up to a limit that easily covers ~10 000 terms */
static void build_sequence(void)
{
    /* A pragmatic upper bound.  For 10 000 terms the count of prime pairs
       at distance 38 is well below 20 000, so 20 000 000 is sufficient. */
    const idx_t LIMIT = 20000000;
    char *is_prime = (char *)malloc((LIMIT + 1) * sizeof(char));
    if (!is_prime) return;          /* silently fail – caller may retry */

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect primes p with p + 38 also prime  */
    for (idx_t p = 3; p + 38 <= LIMIT; p += 2) {
        if (is_prime[p] && is_prime[p + 38])
            push_seq((seq_t)p);
    }

    free(is_prime);
}

/* public sequence accessor */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_sequence();
        initialized = true;
    }
    /* a(1) is the first term; if n is out of range return 0 (undefined)
       behaviour outside the defined offset is unspecified. */
    if (n == 0 || n > seq_len) return 0;   /* undefined */
    return seq[n - 1];
}
