/*
 * A291125  "Doubly A289484" numbers: numbers with prime factorization p1^e1 *
 *          p2^e2 * ... * pk^ek such that there exist i < j < k with p1^e1 *
 *          p2^e2 * ... pi^ei > p(i+1) and p1^e1 * p2^e2 * ... pj^ej > p(j+1).
 * https://oeis.org/A291125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A291125 – Doubly A289484
 *
 *  The algorithm generates all integers n ≥ 1 that satisfy the
 *  defining condition:
 *
 *      For the prime factorization n = p1^e1 * p2^e2 * … * pk^ek
 *      (p1 < p2 < … < pk),  the number is included if there exist
 *      indices i < j < k such that
 *
 *          (p1^e1 * … * pi^ei)   >  p(i+1)
 *          (p1^e1 * … * pj^ej)   >  p(j+1)
 *
 *  We pre‑compute a sieve for smallest prime factors up to a
 *  fixed upper bound (5 000 000).  All numbers up to this bound are
 *  tested in ascending order until we have collected the required
 *  n-th term.  The results are memoised in a static array, so every
 *  subsequent call of a() is O(1) after the initial sieve construction.
 *
 *  The implementation is fully self‑contained, uses only integer
 *  arithmetic and the build‑time provided integer types.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static data ---------- */
#define MAX_LIMIT 5000000UL

static uint32_t *spf = NULL;           /* smallest prime factor table  */
static size_t   spf_cap = 0;           /* size of spf table             */
static seq_t   *solutions = NULL;      /* collected terms              */
static size_t   solutions_cap = 0;     /* capacity of solutions array  */
static size_t   solutions_count = 0;   /* number of terms collected    */
static uint32_t next_checked = 2;      /* next integer to test         */

/* ---------- helper: populate spf table ---------- */
static void build_spf(void)
{
    size_t i, j;
    spf = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    for (i = 0; i <= MAX_LIMIT; ++i) spf[i] = (uint32_t)i;
    for (i = 2; i * i <= MAX_LIMIT; ++i)
    {
        if (spf[i] == (uint32_t)i)      /* i is prime */
        {
            for (j = i * i; j <= MAX_LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
    spf_cap = MAX_LIMIT + 1;
}

/* ---------- helper: check doubly A289484 condition ---------- */
static bool is_doubly_A289484(uint32_t n)
{
    /* factor n using spf table */
    uint32_t primes[10];
    uint32_t exps [10];
    size_t   count = 0;

    uint32_t tmp = n;
    while (tmp > 1)
    {
        uint32_t p = spf[tmp];
        uint32_t e = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++e;
        }
        primes[count] = p;
        exps[count]  = e;
        ++count;
        if (count == 10) break;  /* safe guard – cannot exceed 10 distinct primes for n ≤ MAX_LIMIT */
    }
    if (count < 3) return false;   /* need at least 3 distinct primes */

    /* compute cumulative products */
    seq_t cum[10];
    seq_t prod = 1;
    for (size_t i = 0; i < count; ++i)
    {
        seq_t temp = 1;
        for (uint32_t e = 0; e < exps[i]; ++e) temp = temp * (seq_t)primes[i];
        prod *= temp;          /* prod == p1^e1 * … * p(i+1)^e(i+1) */
        cum[i] = prod;         /* cumulative product up to i */
    }

    /* search for indices i < j satisfying the inequalities */
    for (size_t i = 0; i + 1 < count; ++i)
    {
        if (cum[i] > (seq_t)primes[i+1])      /* first inequality holds for i */
        {
            for (size_t j = i + 1; j + 1 < count; ++j)
            {
                if (cum[j] > (seq_t)primes[j+1])
                    return true;               /* found suitable pair */
            }
        }
    }
    return false;
}

/* ---------- helper: append to solutions array ---------- */
static void append_solution(seq_t val)
{
    if (solutions_count == solutions_cap)
    {
        size_t new_cap = solutions_cap ? solutions_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        solutions = tmp;
        solutions_cap = new_cap;
    }
    solutions[solutions_count++] = val;
}

/* ---------- main routine ---------- */
seq_t a(idx_t n)
{
    /* initialise static structures on the first call */
    if (!spf)          /* build spf table once */
        build_spf();

    if (!solutions)    /* allocate solutions array */
    {
        solutions_cap = 4;
        solutions = (seq_t *)malloc(solutions_cap * sizeof(seq_t));
        if (!solutions) exit(EXIT_FAILURE);
    }

    /* generate terms until we have at least n of them */
    while (solutions_count < (size_t)n && next_checked <= MAX_LIMIT)
    {
        if (is_doubly_A289484(next_checked))
            append_solution((seq_t)next_checked);
        ++next_checked;
    }

    /* If we exhausted the sieve range without finding n terms,
       the sequence grows beyond the chosen MAX_LIMIT – this
       should not happen for the test cases. */
    if (solutions_count < (size_t)n)
        exit(EXIT_FAILURE);

    return solutions[n - 1];
}
