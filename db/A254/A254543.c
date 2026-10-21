/*
 * A254543  Number of length 5 1..(n+2) arrays with no leading partial sum
 *          equal to a prime.
 * https://oeis.org/A254543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of length‑5 arrays with entries 1..(n+2) whose every
 *            leading partial sum is not prime
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 103, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  prime sieve (static state)  ---------- */
static bool *prime_table = NULL;   /* prime_table[s] == true  => s is prime  */
static size_t prime_cap  = 0;      /* allocated size of prime_table   */

static void ensure_primes(size_t limit)
/* build sieve up to 'limit' (inclusive) if not already available */
{
    if (limit + 1 <= prime_cap) return;          /* already good */

    bool *new_table = (bool *)calloc(limit + 1, sizeof(bool));
    if (!new_table) abort();                    /* OOM protection */

    /* 0 and 1 are not prime */
    new_table[0] = false;
    new_table[1] = false;
    /* mark all as potential primes */
    for (size_t i = 2; i <= limit; ++i) new_table[i] = true;
    /* Sieve */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_table[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                new_table[j] = false;
        }
    }
    free(prime_table);
    prime_table = new_table;
    prime_cap   = limit + 1;
}

/* ----------  DP arrays (static state)  ---------- */
static seq_t *dp_prev = NULL;
static seq_t *dp_cur  = NULL;
static size_t dp_cap  = 0;   /* current allocation size for dp arrays (max_sum+1) */

/* ----------  main function  ---------- */
seq_t a(idx_t n)
{
    /* domain parameters */
    size_t M = (size_t)(n + 2);          /* maximum element value */
    size_t max_sum = 5 * M;              /* maximum possible partial sum */

    /* ensure prime table covers all possible sums */
    ensure_primes(max_sum);

    /* ensure DP arrays large enough */
    if (max_sum + 1 > dp_cap) {
        free(dp_prev);
        free(dp_cur);
        dp_prev = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
        dp_cur  = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
        if (!dp_prev || !dp_cur) abort();  /* OOM protection */
        dp_cap = max_sum + 1;
    }

    /* ----------  initialise for the first element  ---------- */
    /* zero dp_prev has been done by calloc */

    for (size_t s = 1; s <= M; ++s) {
        if (!prime_table[s])            /* S_1 must not be prime */
            dp_prev[s] = 1;
    }

    /* ----------  iterate over remaining 4 positions  ---------- */
    for (int step = 2; step <= 5; ++step) {
        seq_t window = 0;               /* sliding window sum over dp_prev */

        for (size_t s = 0; s <= max_sum; ++s) {
            if (s > 0) {                /* add new candidate ending with value s-(s-1) */
                window += dp_prev[s - 1];
            }
            if (s >= M + 1) {           /* remove value that fell out of the window */
                window -= dp_prev[s - M - 1];
            }

            /* If the new partial sum s is prime, no sequence can end here */
            if (s == 0 || prime_table[s])
                dp_cur[s] = 0;
            else
                dp_cur[s] = window;
        }
        /* swap dp_prev and dp_cur for next step */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    /* ----------  sum all valid final partial sums  ---------- */
    seq_t total = 0;
    for (size_t s = 0; s <= max_sum; ++s) {
        total += dp_prev[s];
    }

    return total;
}
