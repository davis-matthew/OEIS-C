/*
 * A381242  Lexicographically earliest sequence of distinct terms > 1 such
 *          that no term is a substring of the product of any two terms.
 * https://oeis.org/A381242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Alexicographically earliest sequence of distinct terms >1 such
 *            that no term is a substring of the product of any two terms. The
 *            implementation generates the sequence by trying consecutive
 *            integers, rejecting those that violate the substring‑condition.
 *            Termination for n ≥ 1 is guaranteed
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 35, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/to_str.h"

static seq_t      *g_terms   = NULL;   /* numeric values of the terms   */
static char      **g_strs    = NULL;   /* decimal strings of the terms  */
static size_t    *g_lens    = NULL;   /* lengths of the decimal strings*/
static size_t      g_cap     = 0;      /* allocated capacity of the above arrays */
static size_t      g_len     = 0;      /* number of terms already stored  */
static seq_t      g_next    = (seq_t)2;/* next integer to test             */

/* Make sure the dynamic arrays have space for at least 'need' elements   */
static void ensure_capacity(size_t need)
{
    if (need <= g_cap) return;
    size_t newcap = g_cap ?: 8;
    while (newcap < need) newcap <<= 1;
    g_terms = (seq_t*)realloc(g_terms, newcap * sizeof(seq_t));
    g_strs  = (char**)realloc(g_strs,  newcap * sizeof(char*));
    g_lens  = (size_t*)realloc(g_lens,  newcap * sizeof(size_t));
    g_cap   = newcap;
}

/* Check whether a candidate value satisfies the substring condition.   */
static bool candidate_ok(seq_t cand)
{
    /* Convert candidate to string once, for digit checking and later use */
    char *cand_str = to_str(cand);
    size_t cand_len = strlen(cand_str);

    /* Reject if the decimal representation contains forbidden digits 1 or 6 */
    for (size_t i = 0; i < cand_len; ++i) {
        if (cand_str[i] == '1' || cand_str[i] == '6') {
            free(cand_str);
            return false;
        }
    }

    /* Test with all existing terms (prefix terms) */
    for (size_t i = 0; i < g_len; ++i) {
        seq_t prod = g_terms[i] * cand;
        char *prod_str = to_str(prod);
        size_t prod_len = strlen(prod_str);

        /* Check substrings against earlier terms */
        for (size_t j = 0; j < g_len; ++j) {
            if (g_lens[j] > prod_len) continue;
            if (strstr(prod_str, g_strs[j]) != NULL) {
                free(prod_str);
                free(cand_str);
                return false;
            }
        }
        /* Check against the candidate itself */
        if (cand_len <= prod_len && strstr(prod_str, cand_str) != NULL) {
            free(prod_str);
            free(cand_str);
            return false;
        }
        free(prod_str);
    }

    /* Test with the candidate paired with itself */
    seq_t prod = cand * cand;
    char *prod_str = to_str(prod);
    size_t prod_len = strlen(prod_str);
    for (size_t j = 0; j < g_len; ++j) {
        if (g_lens[j] > prod_len) continue;
        if (strstr(prod_str, g_strs[j]) != NULL) {
            free(prod_str);
            free(cand_str);
            return false;
        }
    }
    if (cand_len <= prod_len && strstr(prod_str, cand_str) != NULL) {
        free(prod_str);
        free(cand_str);
        return false;
    }
    free(prod_str);
    free(cand_str);
    return true;
}

/*------------------------------------------------------------*/
/*   Public function required by the judge                   */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1; 1‑based index */

    while (g_len < n) {
        seq_t cand = g_next;
        /* Find the next admissible integer */
        while (!candidate_ok(cand)) cand++;
        /* Store the accepted term */
        ensure_capacity(g_len + 1);
        g_terms[g_len] = cand;

        char *cand_str = to_str(cand);
        g_strs[g_len]  = cand_str;
        g_lens[g_len]  = strlen(cand_str);

        g_len++;
        g_next = cand + (seq_t)1;          /* next candidate to try  */
    }
    return g_terms[n-1];
}
