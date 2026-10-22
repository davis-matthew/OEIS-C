/*
 * A395399  Number of unsatisfiable 3-SAT formulas with 3 variables and n
 *          clauses in the multiset clause model.
 * https://oeis.org/A395399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{S⊆{1,…,8}} (-1)^{|S|}·binomial(n + (56-|U(S)|) – 1 ,
 *            n) where U(S) is the union of the 10 clause types falsified by
 *            each assignment in S. This implements the inclusion–exclusion
 *            formula for the number of unsatisfiable 3‑SAT formulas with 3
 *            variables and n clauses
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Constants */
#define VARS          3
#define LITERALS      6   /* {x1,x2,x3,¬x1,¬x2,¬x3} */
#define ASSIGNMENTS   8   /* 2^V */
#define CLAUSE_TYPES  56  /* number of multiset clauses of size 3 */

/* Precomputed data */
static int coeff[CLAUSE_TYPES + 1];   /* coefficient per union size u = |U(S)| */
static bool initialized = false;
static uint8_t clause_assign_mask[CLAUSE_TYPES];  /* bitmask of assignments that falsify the clause */

/* Build masks and coefficients once */
static void build_coeffs(void)
{
    /* Step 1: compute mask of FALSE literals for each assignment */
    uint8_t false_literal_mask[ASSIGNMENTS];
    for (unsigned a = 0; a < ASSIGNMENTS; ++a) {
        uint8_t mask = 0;
        for (unsigned v = 0; v < VARS; ++v) {
            unsigned bit = (a >> v) & 1U;                 /* value of variable v */
            unsigned lit = bit ? (2 * v + 1) : (2 * v);   /* literal that is FALSE under a */
            mask |= (1u << lit);
        }
        false_literal_mask[a] = mask;
    }

    /* Step 2: enumerate all 56 clause types (multisets of 3 literals) */
    unsigned idx = 0;
    for (unsigned i = 0; i < LITERALS; ++i)
        for (unsigned j = i; j < LITERALS; ++j)
            for (unsigned k = j; k < LITERALS; ++k)
            {
                uint8_t clause_mask = (1u << i) | (1u << j) | (1u << k);
                uint8_t assignments = 0;
                for (unsigned a = 0; a < ASSIGNMENTS; ++a)
                    if ((false_literal_mask[a] & clause_mask) == clause_mask)
                        assignments |= (1u << a);
                clause_assign_mask[idx++] = assignments;
            }

    /* Step 3: inclusion–exclusion over assignments */
    for (int i = 0; i <= CLAUSE_TYPES; ++i)
        coeff[i] = 0;
    coeff[0] = 1;  /* empty set */

    for (unsigned subset = 1; subset < (1u << ASSIGNMENTS); ++subset) {
        int pop = __builtin_popcount(subset);
        int sign = (pop % 2 == 0) ? 1 : -1;  /* (-1)^|S| */

        int union_size = 0;
        for (unsigned c = 0; c < CLAUSE_TYPES; ++c)
            if (clause_assign_mask[c] & subset)
                ++union_size;

        coeff[union_size] += sign;
    }

    initialized = true;
}

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for offset 1, safe fallback */
        return 0;

    if (!initialized)
        build_coeffs();

    seq_t result = 0;
    for (int u = 0; u <= CLAUSE_TYPES; ++u) {
        int w = coeff[u];
        if (w == 0)
            continue;

        int allowed = CLAUSE_TYPES - u;
        if (allowed == 0)   /* no clause types left: term is zero for n ≥ 1 */
            continue;

        seq_t term = binomial((idx_t)(n + (idx_t)allowed - 1), n);

        if (w >= 0)
            result += (seq_t)w * term;
        else
            result -= (seq_t)(-w) * term;
    }
    return result;
}
