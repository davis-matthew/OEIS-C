/*
 * A351119  Numbers that are the sum of two balanced numbers in at least one
 *          way.
 * https://oeis.org/A351119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   calc_phi, grow_seq, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A351119: numbers that are the sum of two balanced numbers in at least one way. */
/* A balanced number k satisfies phi(k) | sigma(k). The function builds the sequence on demand. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"
#include "../utils/grow_seq.h"

/* Global state -------------------------------------------------------------- */
static seq_t *seq_arr = NULL;      /* array of computed sums (the OEIS sequence) */
static size_t seq_cap = 0;         /* allocated capacity of seq_arr            */
static size_t seq_size = 0;        /* number of sums currently stored           */

static uint64_t max_bal_checked = 0; /* largest integer for which we have tested balancedness */
static size_t sum_cap = 0;             /* capacity of sum_flag array (0..sum_cap-1)   */
static uint8_t *sum_flag = NULL;      /* sum_flag[s] == 1 if s is a sum of two balanced numbers yet seen */
static uint64_t next_scan = 0;         /* next index to scan in sum_flag            */

static uint64_t bal_cap = 0;          /* capacity of balanced array                 */
static uint64_t bal_size = 0;         /* number of balanced numbers stored          */
static uint64_t *balanced = NULL;     /* array of balanced numbers (uint64_t)       */

static size_t block = 128;            /* size of next range to scan for balanced   */

/* -------------------------------------------------------------------------- */
static void init_state(void)
{
    if (!seq_arr) {
        seq_cap = 256;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    if (!sum_flag) {
        sum_cap = 1;
        sum_flag = (uint8_t *)malloc(sum_cap * sizeof(uint8_t));
        memset(sum_flag, 0, sum_cap);
    }

    if (!balanced) {
        bal_cap = 256;
        balanced = (uint64_t *)malloc(bal_cap * sizeof(uint64_t));
    }

    next_scan = 0;
}

/* -------------------------------------------------------------------------- */
static void extend_balanced(uint64_t new_max)
{
    if (new_max <= max_bal_checked) return;

    /* Ensure sum_flag can hold sums up to 2*new_max */
    uint64_t required_sum_cap = new_max * 2ULL + 1ULL;
    if (required_sum_cap > sum_cap) {
        uint8_t *new_flag = (uint8_t *)realloc(sum_flag, required_sum_cap * sizeof(uint8_t));
        if (!new_flag) abort();
        memset(new_flag + sum_cap, 0, (required_sum_cap - sum_cap) * sizeof(uint8_t));
        sum_flag = new_flag;
        sum_cap = required_sum_cap;
    }

    /* Ensure balanced array can hold new numbers */
    if (new_max - max_bal_checked > bal_cap - bal_size) {
        uint64_t new_bal_cap = bal_cap;
        while (new_max - max_bal_checked > new_bal_cap - bal_size) new_bal_cap *= 2ULL;
        uint64_t *new_bal = (uint64_t *)realloc(balanced, new_bal_cap * sizeof(uint64_t));
        if (!new_bal) abort();
        balanced = new_bal;
        bal_cap = new_bal_cap;
    }

    /* Test numbers from max_bal_checked+1 up to new_max */
    for (uint64_t k = max_bal_checked + 1; k <= new_max; ++k) {
        seq_t phi = calc_phi((seq_t)k);
        seq_t sigma = sigma_of((idx_t)k);
        if (sigma % phi == 0) {            /* k is balanced */
            /* Record sums with all previous balanced numbers, including itself */
            for (uint64_t i = 0; i < bal_size; ++i) {
                uint64_t s = k + balanced[i];
                if (s < sum_cap) sum_flag[s] = 1;
            }
            uint64_t s = k + k;
            if (s < sum_cap) sum_flag[s] = 1;

            /* Store k */
            balanced[bal_size++] = k;
        }
    }

    max_bal_checked = new_max;
}

/* -------------------------------------------------------------------------- */
static void scan_new_sums(void)
{
    for (uint64_t s = next_scan; s < sum_cap; ++s) {
        if (sum_flag[s]) {
            /* Ensure capacity for seq_arr */
            if (seq_size == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 2;
                grow_seq(&seq_arr, &seq_cap, new_cap);
            }
            seq_arr[seq_size++] = (seq_t)s;
        }
    }
    next_scan = sum_cap;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safe for index 0, though offset is 1 */

    init_state();

    /* Keep extending the balanced set until we have at least n sums */
    while (seq_size < (size_t)n) {
        uint64_t new_max = max_bal_checked + block;
        extend_balanced(new_max);
        scan_new_sums();
        block <<= 1;  /* increase block size exponentially */
    }

    /* The sequence is built in ascending order, so return the (n-1)-th element */
    return seq_arr[n - 1];
}
