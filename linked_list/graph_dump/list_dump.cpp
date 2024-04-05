#include "list_dump.h"

int ListDump (const char * fname, List * list, size_t err_vec, ListDebugInfo debug_info)
{
    assert(fname);
    assert(list);

    srand(time(NULL));

    char * dot_code = CreateDotCode(list);

    WriteDotCode(fname, (const char *) dot_code);

    char * command = (char *) calloc (DEFAULT_BUF_SIZE, sizeof(char));

    int dump_id = rand();

    sprintf(command, "dot -Tpng %s -o linked_list/graph_dump/dumps/graph_dump_%d.png", fname, dump_id);
    system(command);

    WriteHTML(HTML_DUMP_FNAME, dump_id, err_vec, debug_info);

    free(command);
    free(dot_code);

    return 0;
}

char * CreateDotCode (const List * list)
{
    assert(list);

    size_t size = DEFAULT_BUF_SIZE * 1000;

    char * dot_code = (char *) calloc(size, sizeof(char));

    char * nodes = CreateNodes (list, size);
    char * vals  = CreateVals  (list, size);
    char * edges = CreateEdges (list, size);

    sprintf(dot_code, "digraph list_%d {\n"
                      "rankdir = LR\n"
                      "splines = ortho\n"
                      "nodesep = 0.8\n"
                      "\tedge[minlen = 1, penwidth = 1.5];\n"
                      "%s"
                      "%s"
                      "%s"
                      "}\n", rand(), nodes, edges, vals);

    free(vals);
    free(nodes);
    free(edges);

    return dot_code;
}

char * CreateVals (const List * list, size_t size)
{
    assert(list);

    char * vals = (char *) calloc(size, sizeof(char));
    char * vals_init = vals;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\"");

    sprintf(vals, "subgraph cluster_val {\n%n", &symbs);
    vals += symbs;

    sprintf(vals, "     val_fre  [ shape = none, %s, fontcolor = black, label = \" fre\n%d \"];\n%n", style, list->fre, &symbs);
    vals += symbs;

    sprintf(vals, "}\n%n", &symbs);
    vals += symbs;

    sprintf(vals, "     val_size  [ shape = none, %s, fontcolor = black, label = \" size\n%d \"];\n%n", style, list->size, &symbs);
    vals += symbs;

    free(style);

    return vals_init;
}

char * CreateNodes (const List * list, size_t size)
{
    assert(list);

    char * nodes = (char *) calloc(size, sizeof(char));
    char * nodes_init = nodes;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));

    sprintf(style, "fontname=\"Helvetica, Arial, sans-serif\""
                   "style=rounded\n"
                   "color = green\n");

    sprintf(nodes, "subgraph cluster_nodes_%d {\n"
                   "bgcolor=\"#B5E2FA\" \n%n", rand(), &symbs);
    nodes += symbs;

    for (int i = 0; i < list->size; i++, symbs = 0)
    {
        if (list->prev[i] != -1)
        {
            sprintf(nodes, "\tnode_%d [%s shape = record, style = filled, fillcolor = \"#4CB944\", label = \" %d | %s (%d) | <fnext> next = %d | <fprev> prev = %d \"];\n%n",
                                                                            i, style, i, list->data[i].word, list->data[i].value, list->next[i], list->prev[i], &symbs);
            nodes += symbs;
        }
        else
        {
            sprintf(nodes, "\tnode_%d [%s shape = record, style = filled, fillcolor = \"#F5EE9E\", label = \" %d | %s (%d) | <fnext> next = %d | <fprev> prev = %d \"];\n%n",
                                                                            i, style, i, list->data[i].word, list->data[i].value, list->next[i], list->prev[i], &symbs);
            nodes += symbs;
        }
    }

    sprintf(nodes, "}\n%n", &symbs);
    nodes += symbs;

    free(style);

    return nodes_init;
}

char * CreateEdges (const List * list, size_t size)
{
    assert(list);

    char * edges = (char *) calloc(size, sizeof(char));
    char * edges_init = edges;

    int symbs = 0;

    char * style = (char *) calloc(STYLE_TAG_SIZE, sizeof(char));

    if (list->fre != list->size)
    {
        sprintf(edges, "val_fre -> node_%d;\n%n", list->fre, &symbs);
        edges += symbs;
    }

    // invisible arrows to align graph elems
    for (int i = 0; i < list->size - 1; i++) {
        sprintf(edges, "\tnode_%d -> node_%d[weight = 10, style = invis];\n %n", i, i + 1, &symbs);
        edges += symbs;
    }

    // next arrows
    for (int i = 0; i < list->size - 1; i++, symbs = 0)
    {
        if (list->next[i] != -1)
        {
            if (list->prev[i] != -1)
            {
                sprintf(edges, "node_%d -> node_%d  [color = blue];\n%n", i, list->next[i], &symbs);
                edges += symbs;
            }
            else
            {
                sprintf(edges, "node_%d -> node_%d  [color = grey];\n%n", i, list->next[i], &symbs);
                edges += symbs;
            }
        }
    }

    for (int i = 0; i < list->size - 1; i++, symbs = 0)
    {
        if (list->prev[i] != -1)
        {
            sprintf(edges, "node_%d -> node_%d  [color = red];\n%n", i, list->prev[i], &symbs);
            edges += symbs;
        }
    }

    free(style);

    return edges_init;
}

int WriteDotCode (const char * fname, const char * dot_code)
{
    assert(fname);
    assert(dot_code);

    FILE * fout = fopen(fname, "wb");

    int ret_code = fprintf(fout, "%s", dot_code);

    fclose(fout);

    return ret_code;
}

int WriteHTML (const char * HTML_fname, int dump_id, size_t err_vec, ListDebugInfo debug_info)
{
    assert(HTML_fname);

    time_t t = time(NULL);

    tm * loc_time = localtime(&t);

    FILE * HTML_file = fopen(HTML_fname, "ab");

    fprintf(HTML_file, "<p style=\"font-family:monospace; font-size: 20px\">[%s]\nLIST DUMP of \"%s\" called from %s (%d) from %s</p>", asctime(loc_time), debug_info.list_name, debug_info.funcname, debug_info.line, debug_info.filename);

    fprintf(HTML_file, "<img src=\"./graph_dump_%d.png\">\n", dump_id);

    fprintf(HTML_file, "<hr>\n");
    fprintf(HTML_file, "<hr>\n");

    fclose(HTML_file);

    return 0;
}
