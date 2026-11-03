/*
 * A294993  Numbers n > 1 such that all of 2^(n-1), 3^(n-1), 5^(n-1),
 *          (2*n-1)*(2^((n-1)/2)), 4*ceiling((3/4)*n)-2, and (2^((n+1)/2) +
 *          floor(n/4)*2^((n+3)/2)) are congruent to 1 (mod n).
 * https://oeis.org/A294993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer > 1 such that all of the * six
 *            expressions * * 2^(n−1) , 3^(n−1) , 5^(n−1) , *
 *            (2·n−1)·2^((n−1)/2) , * 4·ceil(3·n/4)−2 , *
 *            2^((n+1)/2)+floor(n/4)·2^((n+3)/2) * * are congruent to 1 (mod
 *            n)
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Cache of the discovered terms */
static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static size_t terms_len  = 0;

/* Next odd candidate > 1 to test; start with 3. */
static idx_t next_candidate = 3;

/* Append a new term to the cache, enlarging the array if necessary. */
static void
add_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp)
            abort();                       /* memory exhausted */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Return the n‑th term (offset 1).  If the cache does not yet contain
 * enough terms, advance the search until the required count is reached. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset 1 – index 0 is unused */
        return (seq_t)0;

    while (terms_len < n) {
        /* Search the next candidate that satisfies all six congruences. */
        while (true) {
            if ((next_candidate & 1) == 0) {  /* skip even numbers */
                ++next_candidate;
                continue;
            }

            seq_t m = (seq_t)next_candidate;
            seq_t two   = (seq_t)2;
            seq_t three = (seq_t)3;
            seq_t five  = (seq_t)5;

            /* 2^(m-1) ≡ 1 (mod m) */
            if (powmod(two, next_candidate - 1, m) != (seq_t)1) {
                next_candidate += 2;
                continue;
            }

            /* 3^(m-1) ≡ 1 (mod m) */
            if (powmod(three, next_candidate - 1, m) != (seq_t)1) {
                next_candidate += 2;
                continue;
            }

            /* 5^(m-1) ≡ 1 (mod m) */
            if (powmod(five, next_candidate - 1, m) != (seq_t)1) {
                next_candidate += 2;
                continue;
            }

            /* (2*m-1)·2^((m-1)/2) ≡ 1 (mod m) */
            uint64_t half_exp = (next_candidate - 1) / 2;
            seq_t pow_half    = powmod(two, half_exp, m);
            seq_t tmp1        = ((m - (seq_t)1) * pow_half) % m;
            if (tmp1 != (seq_t)1) {
                next_candidate += 2;
                continue;
            }

            /* 4*ceil(3*m/4)−2 ≡ 1 (mod m) */
            seq_t ceil3m4 = (((seq_t)3 * m) + (seq_t)3) / (seq_t)4;
            seq_t val5    = ((((seq_t)4 * ceil3m4) - (seq_t)2) % m);
            if (val5 != (seq_t)1) {
                next_candidate += 2;
                continue;
            }

            /* 2^((m+1)/2)+floor(m/4)·2^((m+3)/2) ≡ 1 (mod m) */
            uint64_t exp1 = (next_candidate + 1) / 2;
            uint64_t exp2 = (next_candidate + 3) / 2;
            seq_t term1   = powmod(two, exp1, m);
            seq_t term2   = ((seq_t)(next_candidate / 4) *
                             powmod(two, exp2, m)) % m;
            seq_t val6    = (term1 + term2) % m;
            if (val6 != (seq_t)1) {
                next_candidate += 2;
                continue;
            }

            /* All conditions satisfied – store the term. */
            add_term(m);
            next_candidate += 2;   /* advance to the next odd candidate */
            break;                /* exit the inner loop */
        }
    }

    return terms[n - 1];
}
