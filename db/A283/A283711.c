/*
 * A283711  Square root of the smallest square referenced in A124809 (Numbers
 *          of the form (square + 1) that are not squarefree).
 * https://oeis.org/A283711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that p² divides m²+1, where m
 *            is the n‑th integer for which m²+1 is not squarefree (i.e., has
 *            a prime square factor)
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* maximum prime we pre‑compute – sufficient for many thousands of terms */
#define PRIME_LIMIT 200000

/* static storage */
static uint32_t primes[PRIME_LIMIT + 1];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static seq_t *memo = NULL;
static size_t memo_size  = 0;
static size_t memo_cap   = 0;

static idx_t current_m = 1;   /* next m to test */

/* --------------------------------------------------------------------- */
/* simple sieve – generate all primes up to PRIME_LIMIT                */
static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    /* simple sieve using a byte array */
    static uint8_t is_comp[PRIME_LIMIT + 1];
    memset(is_comp, 0, sizeof(is_comp));

    for (size_t p = 2; p <= PRIME_LIMIT; ++p) {
        if (!is_comp[p]) {
            primes[prime_cnt++] = (uint32_t)p;
            if (p * p <= PRIME_LIMIT) {
                for (size_t j = p * p; j <= PRIME_LIMIT; j += p)
                    is_comp[j] = 1;
            }
        }
    }

    /* initialise memoised terms array */
    memo_cap = 16;
    memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    if (!memo) abort();
}

/* make sure memo array can hold at least 'need' elements */
static void ensure_memo_cap(size_t need)
{
    if (need <= memo_cap) return;
    size_t new_cap = memo_cap ? memo_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    memo = tmp;
    memo_cap = new_cap;
}

/* return the smallest prime p with p² | N; 0 if none found             */
static seq_t smallest_square_root(seq_t N)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        seq_t p2 = (seq_t)p * p;
        if (p2 > N) break;          /* p² cannot divide N */
        if (N % p2 == 0) return p;
    }
    return 0;                       /* no prime square divisor */
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */

    init_primes();

    /* generate terms until we have at least n cached */
    while (memo_size < (size_t)n) {
        seq_t N = ((seq_t)current_m * current_m) + 1;
        seq_t p = smallest_square_root(N);
        if (p != 0) {
            ensure_memo_cap(memo_size + 1);
            memo[memo_size++] = p;
        }
        ++current_m;
    }
    return memo[n - 1];
}
