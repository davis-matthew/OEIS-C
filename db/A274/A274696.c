/*
 * A274696  Variation on Fermat's Diophantine m-tuple: 1 + the LCM of any two
 *          distinct terms is a square.
 * https://oeis.org/A274696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=3, a(4)=8, a(5)=15, a(6)=24, a(7)=120,
 *            a(8)=168. For n>8, a(n) is computed iteratively as the smallest
 *            integer >a(n‑1) such that for all i<n, lcm(a(n),a(i))+1 is a
 *            perfect square. The algorithm uses the observation that for n≥6
 *            all terms are of the form p^2−1 with p a prime;...
 * Range:     u8 none, u16 n <= 11, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"

static seq_t *seq = NULL;          /* dynamic array holding the sequence */
static size_t seq_cap = 0;         /* current allocated capacity */
static size_t seq_len = 0;         /* number of terms stored */
static seq_t last_prime = 0;       /* prime p_k such that a(k)=p_k^2−1 */

/* helper to initialize the static storage with the first eight terms */
static void init_sequence(void)
{
    static const seq_t first[8] = {0, 1, 3, 8, 15, 24, 120, 168};

    /* Allocate space for the eight known terms */
    ensure_capacity(8, &seq_cap, (void **)&seq, sizeof(seq_t));
    for (size_t i = 0; i < 8; ++i)
        seq[i] = first[i];
    seq_len = 8;

    /* Determine the prime corresponding to a(8)=168 (169=13^2) */
    seq_t v   = seq[7];
    seq_t s   = floor_sqrt(v + 1);
    if (s * s == v + 1 && is_prime_big(s))
        last_prime = s;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                         /* undefined for n<1, return 0 */

    /* One‑time initialization of the first eight terms */
    if (seq_len == 0)
        init_sequence();

    /* Compute additional terms up to n, if necessary */
    while (seq_len < (size_t)n)
    {
        seq_t last_val = seq[seq_len - 1];

        /* Search for the next prime > last_prime satisfying the lcm condition */
        seq_t cand = last_prime + 1;
        while (1)
        {
            if (is_prime_big(cand))
            {
                seq_t m   = cand % last_val;
                seq_t mm  = (m * m) % last_val;
                if (mm == 1)
                    break;
            }
            cand += 1;
        }

        /* Compute the new term as cand^2 - 1 */
        seq_t sq  = seq_mul(cand, cand);
        seq_t next = sq - 1;

        /* Grow the array if needed and store the new term */
        if (seq_len >= seq_cap)
            ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq, sizeof(seq_t));
        seq[seq_len] = next;
        seq_len++;

        /* Update the prime corresponding to the new term */
        last_prime = cand;
    }

    return seq[n - 1];
}
