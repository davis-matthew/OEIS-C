/*
 * A376594  Inflection and undulation points in the sequence of nonsquarefree
 *          numbers (A013929).
 * https://oeis.org/A376594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A376594: Inflection points in the sequence of
    nonsquarefree numbers (A013929).
    a(n) = index i such that the first differences
    of A013929 at positions i and i+1 are equal.
    The sequence is indexed starting at 1 (a(1)=5). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* primes up to 100000 are enough to test nonsquarefreeness
   for the values needed in the benchmark. */
#define PRIME_LIMIT 100000
static int primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    char *is_comp = (char *)malloc((PRIME_LIMIT + 1) * sizeof(char));
    if (!is_comp) abort();
    for (int i = 0; i <= PRIME_LIMIT; ++i) is_comp[i] = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_count++] = i;
            if ((int64_t)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_initialized = true;
}

/* true if n has a square factor (i.e. n is not squarefree) */
static bool is_nonsquarefree(uint64_t n)
{
    if (n < 4) return false; /* 1,2,3 are squarefree */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = (uint64_t)primes[i];
        uint64_t pp = p * p;
        if (pp > n) break;
        if (n % pp == 0) return true;
    }
    return false;
}

/* array that stores indices of all found inflection points */
static idx_t *plateau_indices = NULL;
static size_t plateau_len = 0;
static size_t plateau_cap = 0;

static void ensure_plateau_cap(size_t needed)
{
    if (needed <= plateau_cap) return;
    size_t new_cap = plateau_cap ? plateau_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(plateau_indices, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    plateau_indices = tmp;
    plateau_cap = new_cap;
}

/* state while generating nonsquarefree numbers */
static seq_t current_number = 1;   /* next integer to test */
static seq_t last_nsf = 0;         /* last found nonsquarefree number */
static idx_t nonsq_count = 0;      /* number of nonsquarefree numbers seen */
static seq_t prev_diff = 0;        /* previous first difference */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1 */

    init_primes();

    /* generate until we have at least n inflection indices stored */
    while (plateau_len < (size_t)n) {
        /* find next nonsquarefree number */
        do { ++current_number; } while (!is_nonsquarefree((uint64_t)current_number));

        ++nonsq_count;   /* index of the newly found nonsquarefree number  */

        if (last_nsf != 0) {
            seq_t diff = current_number - last_nsf;
            if (nonsq_count >= 3) {
                if (diff == prev_diff) {
                    /* plateau at index = nonsq_count - 2 */
                    ensure_plateau_cap(plateau_len + 1);
                    plateau_indices[plateau_len++] = nonsq_count - 2;
                }
            }
            prev_diff = diff;
        }
        last_nsf = current_number;
    }

    return (seq_t)plateau_indices[n - 1];
}
