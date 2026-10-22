/*
 * A363250  Numbers in A363063 arranged in lexicographic order according to
 *          ordered prime signature (i.e., multiplicities of prime power
 *          factors p^k, written in order of p).
 * https://oeis.org/A363250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 76, u64 n <= 283,
 *            u128 n <= 3520, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A363250
 *  Sequence: Numbers in A363063 arranged in lexicographic order
 *  according to ordered prime signature.
 *  a(0) = 1
 *
 *  Method: Generate exponent vectors (e1, e2, …) such that
 *  for each i:  primes[i+1] ^ e_{i+1} < primes[i] ^ e_i.
 *  Enumerate all such vectors in lexicographic order:
 *  for a fixed prefix, first output the vector itself,
 *  then recur for the next prime.  For each vector produce
 *  the product ∏ primes[i]^{e_i}.  Count the products starting
 *  at 1 (which is the empty vector); stop when the required
 *  index is reached.  A simple sieve supplies a static list
 *  of primes large enough for n up to at least 10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime generation – simple sieve up to a fixed limit.               */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;   /* already done */

    /* upper bound for primes – enough for the required n */
    const uint64_t LIMIT = 50000;

    /* sieve */
    uint8_t *comp = (uint8_t*)calloc(LIMIT + 1, 1);
    uint64_t *temp  = (uint64_t*)malloc((LIMIT / 10) * sizeof(uint64_t));
    size_t cnt = 0;

    for (uint64_t p = 2; p <= LIMIT; ++p)
    {
        if (!comp[p])
        {
            temp[cnt++] = p;
            if (p * p <= LIMIT)
                for (uint64_t q = p * p; q <= LIMIT; q += p)
                    comp[q] = 1;
        }
    }
    free(comp);

    prime_list  = (uint64_t*)realloc(temp, cnt * sizeof(uint64_t));
    prime_count = cnt;
}

/* ------------------------------------------------------------------ */
/*  Recursive enumeration of exponent vectors                         */
static void rec_generate(size_t prime_idx,
                         seq_t prod,
                         seq_t prev_power,
                         idx_t target,
                         idx_t *count,
                         seq_t *result,
                         bool *found)
{
    if (*found || prime_idx >= prime_count)
        return;

    uint64_t p = prime_list[prime_idx];
    seq_t pow  = (seq_t)p;                     /* p^1 */

    for (int exp = 1; ; ++exp)
    {
        if (pow >= prev_power)                /* stop if next power is not < previous */
            break;

        seq_t new_prod = prod * pow;          /* multiply current product by p^exp */

        ++(*count);                           /* a new term is produced */
        if (*count == target)                 /* we reached the desired index */
        {
            *result = new_prod;
            *found  = true;
            return;
        }

        /* Recurse for the next prime */
        rec_generate(prime_idx + 1,
                     new_prod,
                     pow,               /* previous power becomes current */
                     target,
                     count,
                     result,
                     found);
        if (*found)
            return;

        /* Compute the next power of p (p^(exp+1)) */
        if (sizeof(seq_t) == sizeof(uint64_t))
        {
            /* 64‑bit case – guard against overflow */
            unsigned __int128 next = (unsigned __int128)pow * (unsigned __int128)p;
            if (next >= (unsigned __int128)prev_power)
                break;                       /* next power too large */
            pow = (seq_t)next;
        }
        else
        {
            /* arbitrary‑precision type – rely on operator* implementation */
            pow = pow * (seq_t)p;
            if (pow >= prev_power)
                break;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public function requested by the problem                         */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1, the empty product */

    init_primes();                /* ensure prime list is ready */

    idx_t   count  = 0;           /* number of terms produced so far (excluding a(0)) */
    seq_t   res    = 0;           /* value to return when target reached */
    bool    found  = false;       /* flag set when desired term is found */

    seq_t  root_prod      = (seq_t)1;
    seq_t  root_prev_pow  = (seq_t)UINT64_MAX;   /* sentinel – effectively ∞ */

    rec_generate(0,
                 root_prod,
                 root_prev_pow,
                 n,
                 &count,
                 &res,
                 &found);

    /* The problem guarantees that the required term will be found. */
    return res;
}
