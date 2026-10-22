/*
 * A321301  Lexicographically last sequence of positive integers whose terms
 *          can be grouped and summed to produce the natural numbers as well
 *          as the prime numbers.
 * https://oeis.org/A321301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 198, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321301
   Formula: a(n) is the lexicographically last sequence of positive
   integers whose terms can be partitioned into contiguous blocks that
   sum to the natural numbers and also to the prime numbers.
   The sequence is generated greedily: at each step the next term is
   the largest positive integer that does not exceed the remaining
   deficits of the current natural‑number block and the current prime‑
   number block.  This greedy rule yields the lexicographically last
   sequence. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_TERMS 200000          /* safe upper bound for n (≈10 000) */
#define PRIME_LIMIT 200000        /* sieve upper bound for primes */

/* --- state of the sequence construction -------------------------------- */
static seq_t  seq_arr[MAX_TERMS];
static size_t seq_len = 0;         /* number of terms already generated */

static seq_t  target_nat   = 1;    /* current natural number to hit */
static seq_t  sum_nat      = 0;    /* sum accumulated in current nat block */

static seq_t  target_prime = 0;    /* current prime to hit   */
static seq_t  sum_prime    = 0;    /* sum accumulated in current prime block */
static size_t prime_idx    = 0;    /* index of target_prime in primes_arr */

/* --- prime table ------------------------------------------------------- */
static uint64_t primes_arr[PRIME_LIMIT];
static size_t   primes_cnt    = 0;
static bool     primes_ready  = false;

/* ---- sieve ------------------------------------------------------------ */
static void init_primes(void)
{
    if (primes_ready) return;
    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    primes_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes_arr[primes_cnt++] = i;
    free(is_prime);
    primes_ready = true;
}

/* ---- lazy initialization of sequence start state --------------------- */
static void init_sequence_state(void)
{
    if (seq_len > 0) return; /* already initialized */
    init_primes();
    target_nat   = 1;
    sum_nat      = 0;
    if (primes_cnt == 0) exit(1);   /* prime list must exist */
    target_prime = (seq_t)primes_arr[0];
    sum_prime    = 0;
    prime_idx    = 0;
}

/* ---- compute a(n) ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 but guard */
    if ((size_t)n > MAX_TERMS) {
        /* We limit to MAX_TERMS; larger n would require a larger buffer */
        exit(1);
    }
    init_sequence_state();

    /* generate terms up to n */
    for (size_t idx = seq_len; idx < (size_t)n; ++idx) {
        seq_t rem_nat   = target_nat   - sum_nat;
        seq_t rem_prime = target_prime - sum_prime;
        seq_t term = rem_nat < rem_prime ? rem_nat : rem_prime;
        seq_arr[idx] = term;

        /* update natural block */
        sum_nat += term;
        if (sum_nat == target_nat) {
            sum_nat   = 0;
            ++target_nat;
        }

        /* update prime block */
        sum_prime += term;
        if (sum_prime == target_prime) {
            sum_prime = 0;
            ++prime_idx;
            if (prime_idx < primes_cnt)
                target_prime = (seq_t)primes_arr[prime_idx];
            else
                /* should never happen for tested ranges */
                exit(1);
        }
    }
    seq_len = (size_t)n;
    return seq_arr[(size_t)n - 1];
}
