/*
 * A352353  Primes "r" corresponding to the even numbers with exactly 1 pair
 *          of Goldbach partitions, (p,q) and (r,s) with p,q,r,s prime and p <
 *          r <= s < q, such that all integers in the open intervals (p,r) and
 *          (s,q) are composite.
 * https://oeis.org/A352353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A352353 – primes r corresponding to even numbers with a unique pair of
    Goldbach partitions satisfying the composite‑interval condition.
    The algorithm iterates through even numbers in ascending order, finds
    the pair of consecutive prime gaps that sum to the same even number,
    and outputs the smaller second prime r if such a pair exists
    uniquely.  This yields the terms of the sequence in the order
    required by OEIS.  The routine expands the prime sieve as needed.
    The implementation is purely C11/C++17 compatible and uses only
    integer arithmetic. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------- */
/*  Prime sieve:  generate primes up to a given limit.  The sieve
    buffer holds boolean flags that are used only during construction.
    The list of primes is stored in 'primes' (uint64_t).             */
/* --------------------------------------------------------------- */

static uint32_t *primes  = NULL;          /* list of primes          */
static size_t   primes_count  = 0;        /* number of primes        */
static idx_t    prime_limit   = 0;        /* current sieve limit     */

/* Build or extend the sieve up to at least 'limit'.  This function
   reallocates the prime array and recomputes the sieve. */
static void build_sieve(idx_t limit)
{
    if (limit <= prime_limit) return;    /* already sufficient */

    /* free previous resources */
    free(primes);
    primes = NULL;

    prime_limit = limit;
    size_t lim = (size_t)limit + 1U;      /* include prime_limit */

    uint8_t *sieve = (uint8_t*)calloc(lim, sizeof(uint8_t));
    if (!sieve) exit(EXIT_FAILURE);

    /* simple Eratosthenes sieve: 1 means prime initially */
    for (size_t i = 2; i < lim; ++i) sieve[i] = 1;
    for (size_t i = 2; i * i < lim; ++i)
        if (sieve[i])
            for (size_t j = i * i; j < lim; j += i)
                sieve[j] = 0;

    /* count primes and allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i < lim; ++i) if (sieve[i]) ++cnt;
    primes_count = cnt;
    primes = (uint32_t*)malloc(primes_count * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    /* fill primes array */
    size_t pos = 0;
    for (size_t i = 2; i < lim; ++i)
        if (sieve[i]) primes[pos++] = (uint32_t)i;

    free(sieve);
}

/* --------------------------------------------------------------- */
/*  Binary search in the primes array.  Returns the index of the
    prime equal to 'x', or -1 if it is not present.                */
/* --------------------------------------------------------------- */

static int binary_search_prime(uint64_t x)
{
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t mid_val = (uint64_t)primes[mid];
        if (mid_val == x) return (int)mid;
        if (mid_val < x)  lo = mid + 1;
        else              hi = mid;
    }
    return -1;
}

/* --------------------------------------------------------------- */
/*  Data structures for the sequence itself.  seq_vals holds the
    accumulated sequence terms (r values).  The array grows as
    needed.                                                             */
/* --------------------------------------------------------------- */

static seq_t *seq_vals  = NULL;   /* sequence values  */
static size_t seq_len   = 0;      /* number of terms  */
static size_t seq_cap   = 0;      /* array capacity  */

/* --------------------------------------------------------------- */
/*  Iterator state:  even numbers are processed in increasing
    order, starting from 4.  The variable 'next_even' holds the
    next even to examine.                                             */
/* --------------------------------------------------------------- */

static idx_t next_even = 4;

/* --------------------------------------------------------------- */
/*  Ensure that there are at least 'need' elements in seq_vals.   */
/* --------------------------------------------------------------- */

static void ensure_sequence_upto(size_t need)
{
    /* guarantee sufficient primes first */
    if (prime_limit < 1000) build_sieve(1000);

    while (seq_len < need) {
        /* if the current even exceeds the sieve limit, extend it */
        if (next_even > prime_limit - 1) {
            idx_t new_limit = prime_limit * 2 + 1000;
            if (new_limit < next_even) new_limit = next_even + 1000;
            build_sieve(new_limit);
        }

        uint64_t even = (uint64_t)next_even;
        next_even += 2;   /* advance for next call */

        /* Find possible partitions that satisfy the property. */
        size_t count = 0;
        uint64_t r_val = 0;

        /* iterate over the first prime p = primes[i] */
        for (size_t i = 0; i < primes_count && (uint64_t)primes[i] <= even / 2; ++i) {
            uint64_t p = (uint64_t)primes[i];
            uint64_t q = even - p;
            if (q < p) continue;

            int j = binary_search_prime(q);
            if (j == -1 || j < (int)i) continue;

            if ((size_t)i + 1 >= primes_count) continue;
            if ((size_t)j >= 1) {
                size_t j_minus = (size_t)j - 1;
                if (i + 1 <= j_minus) {
                    uint64_t r = (uint64_t)primes[i + 1];
                    uint64_t s = (uint64_t)primes[j_minus];
                    if (r + s == even) {
                        ++count;
                        if (count == 1) r_val = r;
                        if (count > 1) break;   /* no need to search further */
                    }
                }
            }
        }

        if (count == 1) {
            /* append r_val to sequence */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq_vals = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
                if (!seq_vals) exit(EXIT_FAILURE);
                seq_cap = new_cap;
            }
            seq_vals[seq_len++] = (seq_t)r_val;
        }
    }
}

/* --------------------------------------------------------------- */
/*  OEIS entry: A352353.  External function.                      */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t need = (size_t)n;
    ensure_sequence_upto(need);
    return seq_vals[n - 1];
}
