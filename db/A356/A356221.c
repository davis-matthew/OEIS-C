/*
 * A356221  Position of second appearance of 2n in the sequence of prime gaps
 *          A001223; if 2n does not appear at least twice, a(n) = -1.
 * https://oeis.org/A356221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A356221: 
   a(n) = the position (1‑based) of the second appearance of the even
   prime gap 2·n in the sequence of prime gaps.  
   If that even gap never occurs twice, a(n) = -1.  
   The value is computed by generating primes and recording the gaps
   until the second occurrence of each required even gap is known. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system (integer or big‑int). */

/* State for positions of first and second occurrences of even gaps. */
static idx_t *first_pos  = NULL;   /* first_pos[n] = index of first gap 2n */
static idx_t *second_pos = NULL;   /* second_pos[n] = index of second gap 2n */
static size_t pos_len = 0;         /* current allocated length of above arrays */

/* State for prime generation. */
static uint64_t *primes      = NULL;
static size_t primes_cap     = 0;
static size_t primes_cnt     = 0;
static uint64_t cur_candidate = 2;  /* next odd candidate to test */
static uint64_t last_prime     = 0;  /* most recent prime found */
static idx_t cur_gap_index     = 0;  /* number of gaps processed */

/* Ensure position arrays can store indices up to 'need' (1‑based). */
static void ensure_pos_array(idx_t need)
{
    size_t req_len = (size_t)need + 1;      /* include element 0 */
    if (req_len <= pos_len)
        return;

    size_t new_len = pos_len > 0 ? pos_len * 2 : 64;
    if (new_len < req_len)
        new_len = req_len;

    idx_t *tmp1 = (idx_t *)realloc(first_pos, new_len * sizeof(idx_t));
    idx_t *tmp2 = (idx_t *)realloc(second_pos, new_len * sizeof(idx_t));
    first_pos  = tmp1;
    second_pos = tmp2;

    /* Zero‑initialize the newly allocated part. */
    for (size_t i = pos_len; i < new_len; ++i) {
        first_pos[i]  = 0;
        second_pos[i] = 0;
    }
    pos_len = new_len;
}

/* Allocate more space for the prime list. */
static void grow_primes(void)
{
    size_t new_cap = primes_cap == 0 ? 1024 : primes_cap * 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    primes      = tmp;
    primes_cap  = new_cap;
}

/* Generate primes and record gaps until the second occurrence of each
   even gap up to 'n_needed' is known. */
static void generate_up_to(idx_t n_needed)
{
    /* One‑time initialization. */
    if (first_pos == NULL) {
        pos_len = 64;
        first_pos  = (idx_t *)calloc(pos_len, sizeof(idx_t));
        second_pos = (idx_t *)calloc(pos_len, sizeof(idx_t));

        primes_cap = 1024;
        primes = (uint64_t *)calloc(primes_cap, sizeof(uint64_t));
        primes_cnt = 0;
    }

    /* Make sure position arrays can hold the required index. */
    ensure_pos_array(n_needed);

    /* Continue generating until we know the second position for n_needed. */
    while (second_pos[n_needed] == 0) {
        /* Test primality of cur_candidate by trial division. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cur_candidate)
                break;
            if (cur_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store new prime. */
            if (primes_cnt == primes_cap)
                grow_primes();
            primes[primes_cnt++] = cur_candidate;

            /* Update the current gap, if we have a predecessor prime. */
            if (last_prime != 0) {
                ++cur_gap_index;
                uint64_t gap = cur_candidate - last_prime;
                if ((gap & 1ULL) == 0) {                     /* even gap */
                    uint64_t k = gap >> 1;                  /* corresponding n */
                    if (k < pos_len) {
                        if (first_pos[k] == 0)
                            first_pos[k] = cur_gap_index;
                        else if (second_pos[k] == 0)
                            second_pos[k] = cur_gap_index;
                    }
                }
            }
            last_prime = cur_candidate;
        }

        /* Advance to the next odd candidate. */
        do {
            ++cur_candidate;
        } while ((cur_candidate & 1ULL) == 0);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* the sequence is defined for n ≥ 1 only */
        return (seq_t)-1;

    generate_up_to(n);
    return (second_pos[n] == 0) ? (seq_t)-1 : (seq_t)second_pos[n];
}
