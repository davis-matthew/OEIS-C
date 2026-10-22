/*
 * A346338  Numbers that are the sum of nine fifth powers in exactly three
 *          ways.
 * https://oeis.org/A346338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth number that can be written as the sum of nine
 *            fifth powers * in exactly three unordered ways. This
 *            implementation enumerates all unordered * 9‑tuples of positive
 *            integers up to 18, counts the frequencies of the * resulting
 *            sums, and returns the nth sum that occurs exactly three times
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper: compare two seq_t values for qsort. */
static int cmp_seq_t(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    return (*a < *b) ? -1 : (*a > *b);
}

/* Helper: small binomial calculation for 26 choose 9. */
static size_t small_binom(size_t n, size_t k)
{
    if (k > n - k)
        k = n - k;
    size_t r = 1;
    for (size_t i = 1; i <= k; ++i) {
        r = r * (n - k + i) / i;
    }
    return r;
}

/* Recursion generates all unordered 9‑tuples of values 1 … 18. */
#define MAX_VAL 18

/* Globals used during generation. */
static seq_t *all_sums = NULL;   /* array of all sums */
static size_t total_sums = 0;    /* number of sums generated */

static seq_t pow5[MAX_VAL + 1];  /* pow5[i] = i^5 */

/* Recursive generator.
 * idx   : current integer value (1 … 18)
 * rem   : remaining positions to fill in the 9‑tuple
 * sum   : current accumulated sum */
static void gen_sums(unsigned int idx, unsigned int rem, seq_t sum)
{
    if (idx > MAX_VAL) {
        if (rem == 0) {
            all_sums[total_sums++] = sum;
        }
        return;
    }
    /* Try to use idx^5 0 to rem times. */
    const seq_t base = pow5[idx];
    for (unsigned int use = 0; use <= rem; ++use) {
        seq_t new_sum = sum;
        if (use != 0) {
            new_sum = sum + (seq_t)use * base;
        }
        gen_sums(idx + 1, rem - use, new_sum);
    }
}

/* Static storage for the sequence. */
static seq_t *seq_terms = NULL;   /* sorted list of terms */
static size_t seq_len   = 0;      /* number of terms found */

/* Initialise the sequence on first use. */
static void initialise_sequence(void)
{
    /* Pre‑compute fifth powers 1^5 … 18^5. */
    for (unsigned int i = 1; i <= MAX_VAL; ++i) {
        seq_t v = (seq_t)i;
        v *= v;     /* i^2 */
        v *= v;     /* i^4 */
        v *= (seq_t)i;  /* i^5 */
        pow5[i] = v;
    }

    /* Allocate space for all possible sums. */
    size_t comb = small_binom(MAX_VAL + 9 - 1, 9);  /* C(26,9) = 3,124,550 */
    all_sums = (seq_t *)malloc(comb * sizeof(seq_t));
    if (all_sums == NULL) {
        /* Allocation failed – abort gracefully. */
        exit(1);
    }
    total_sums = 0;

    /* Generate all unordered 9‑tuples of values 1…18. */
    gen_sums(1, 9, (seq_t)0);

    /* Sanity check – should equal comb. */
    if (total_sums != comb) {
        free(all_sums);
        all_sums = NULL;
        return;
    }

    /* Sort the sums to count frequencies. */
    qsort(all_sums, total_sums, sizeof(seq_t), cmp_seq_t);

    /* Count frequencies and collect sums that occur exactly three times. */
    seq_terms = NULL;
    seq_len   = 0;
    size_t cap = 1024;      /* initial capacity, will grow as needed */
    seq_terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (seq_terms == NULL) {
        free(all_sums);
        all_sums = NULL;
        exit(1);
    }

    for (size_t i = 0; i < total_sums; ) {
        size_t j = i + 1;
        while (j < total_sums && all_sums[j] == all_sums[i]) {
            ++j;
        }
        size_t freq = j - i;
        if (freq == 3) {
            if (seq_len == cap) {
                cap *= 2;
                seq_terms = (seq_t *)realloc(seq_terms, cap * sizeof(seq_t));
                if (seq_terms == NULL) {
                    free(all_sums);
                    all_sums = NULL;
                    exit(1);
                }
            }
            seq_terms[seq_len++] = all_sums[i];
        }
        i = j;
    }

    free(all_sums);
    all_sums = NULL;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        initialise_sequence();
        initialized = 1;
    }
    if (n == 0 || n > seq_len) {
        return (seq_t)0;  /* undefined for out‑of‑range indices */
    }
    return seq_terms[n - 1];
}
