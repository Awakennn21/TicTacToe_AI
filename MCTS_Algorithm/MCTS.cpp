#include "MCTS.h"
int MCTS::RunMCTS(Node* state, size_t iterations, int playerToBeAsserted)
{
    srand(time(NULL));
    if (state == nullptr)
    {
        return rand() % (MAP_SIZE * MAP_SIZE);
    }
    PlayerToBeAsserted = playerToBeAsserted;
    for (int i = 0; i < iterations; i++)
    {
        SelectNode(state);
    }
    AnalizeMoves(state);
    int Result = ReturnBestMove(state);
    return Result;
}

void MCTS::AnalizeMoves(Node* root)
{
    if (root == nullptr)
    {
        return;
    }

    std::vector<Node*> stack;
    stack.push_back(root);

    while (!stack.empty()) 
    {
        Node* current_node = stack[stack.size() - 1];
        stack.pop_back();

        if (current_node->WinPropability == 0.0 && current_node->children.size() == 0)
        {
            current_node->Parent->WinPropability = 0.0;
        }

        for (auto it : current_node->children)
        {
            stack.push_back(it);
        }
    }    
}

std::vector<std::vector<int>> MCTS::RecreatePos(Node* start)
{
    std::vector<int> current_map(MAP_SIZE * MAP_SIZE, 0);
    if (start != nullptr)
    {
        Node tmp = *start;
        while (tmp.Parent != nullptr)
        {
            current_map[tmp.Move.pos] = tmp.Move.player;
            tmp = *(tmp.Parent);
        }
        current_map[tmp.Move.pos] = tmp.Move.player;
    }

    std::vector<std::vector<int>> CurrentPos;
    for (int i = 0; i < MAP_SIZE; i++)
    {
        CurrentPos.push_back(std::vector<int>());
        for (int j = 0; j < MAP_SIZE; j++)
        {
            CurrentPos[i].push_back(current_map[i * MAP_SIZE + j]);
        }
    }
    return CurrentPos;
}

void MCTS::Clear()
{
    for (auto i : m_AllAlocations)
    {
        delete i;
    }
    m_AllAlocations.clear();
}

std::optional<double> MCTS::HasGameEnded(const std::vector<std::vector<int>>& currentPos)
{
    for (int y = 0; y < currentPos.size(); y++)
    {
        for (int x = 0; x < currentPos[y].size(); x++)
        {
            if (currentPos[y][x] != 0)
            {
                if (InRowCount(currentPos, x, y, 1, 0) >= WIN_CONDITION  ||
                    InRowCount(currentPos, x, y, -1, 1) >= WIN_CONDITION ||
                    InRowCount(currentPos, x, y, 0, 1) >= WIN_CONDITION  ||
                    InRowCount(currentPos, x, y, 1, 1) >= WIN_CONDITION)
                {
                    if (currentPos[y][x] == PlayerToBeAsserted)
                    {
                        return 100;
                    }
                    else
                    {
                        return -100;
                    }
                }
            }
        }
    }
    if (EmptySpacesOnMap(currentPos) == 0)
    {
        return 0;
    }
    else
    {
        return {};
    }
}

void MCTS::SelectNode(Node* start)
{
    Node* best_child = nullptr;
    while (!(start->children.empty()))
    {
        double best_score = -999;
        double score = -1.0;

        for (size_t i = 0; i < start->children.size(); i++)
        {
            if (start->children[i]->Visits != 0)
            {
                double exploitation_value = start->children[i]->WinsCount / start->children[i]->Visits / start->children[i]->Visits;
                double exploration_value = (sqrt(log(start->Visits) / start->children[i]->Visits));
                score = exploitation_value + CONSTANT_C * exploration_value;
            }
            else
            {
                score = 9999999;
            }

            if (score > best_score)
            {
                best_score = score;
                best_child = start->children[i];
            }
        }

        start = best_child;
    }
    std::vector<std::vector<int>> Map = RecreatePos(start);
    if (start->Visits == 0)
    {
        RollOut(start);
    }
    else if (!HasGameEnded(Map).has_value())
    {
        Expansion(start);
    }

}

void MCTS::Expansion(Node* node)
{
    std::vector<MoveInfo> valid_moves = GetValidMoves(node);
    for (const MoveInfo& move : valid_moves)
    {
        AddChildNode(node, move);
    }

    if (valid_moves.empty())
    {
        RollOut(node);
    }
    else
    {
        RollOut(node->children[0]);
    }
}

void MCTS::RollOut(Node* start)
{
    int WinsCount = 0;
    for (int i = 0; i < RolloutIterations; i++)
    {
        std::vector<std::vector<int>> CurrentGameState = RecreatePos(start);
        int PlayerToMove = start->Move.player == 1 ? 2 : 1;
        std::vector<MoveInfo> SpacesAvailable = GetValidMoves(start);
        std::optional<double> Result = HasGameEnded(CurrentGameState);
        while (!Result.has_value() && SpacesAvailable.size() > 0)
        {
            int Random = rand() % SpacesAvailable.size();

            CurrentGameState[SpacesAvailable[Random].pos / MAP_SIZE][SpacesAvailable[Random].pos % MAP_SIZE] = PlayerToMove;
            SpacesAvailable.erase(SpacesAvailable.begin() + Random);

            Result = HasGameEnded(CurrentGameState);

            PlayerToMove = PlayerToMove == 1 ? 2 : 1;
        }
        WinsCount = Result == 100 ? WinsCount + 1 : Result == -100 ? WinsCount - 1 : WinsCount;
    }
    BackPropagate(start, WinsCount);
}

void MCTS::BackPropagate(Node* start, int wins)
{
    while(true)
    {
        start->WinsCount += wins;
        start->Visits++;
        start->WinPropability = 0.5 + 0.5 * (start->WinsCount / start->Visits * RolloutIterations)/100;
        if (start->Parent == nullptr)
        {
            return;
        }
        else
        {
            start = start->Parent;
        }
    } 

}

int MCTS::ReturnBestMove(Node* root)
{
    Node* node = root;
    Node* best_child = root;
    double best_score = -999999;
    for (auto& child : node->children)
    {
        if (child->WinPropability > best_score)
        {
            best_child = child;
            best_score = child->WinPropability;
        }
        else if (child->WinPropability == best_score)
        {
            if (child->Visits > best_child->Visits)
            {
                best_child = child;
                best_score = child->WinPropability;
            }
        }
    }
    return best_child->Move.pos;
}

std::vector<MCTS::MoveInfo> MCTS::GetValidMoves(Node* target)
{
    std::vector<std::vector<int>> Map = RecreatePos(target);
    std::vector<MoveInfo> Result;
    if (!HasGameEnded(Map).has_value())
    {
        for (int i = 0; i < Map.size(); i++)
        {
            for (int j = 0; j < Map[i].size(); j++)
            {
                if (Map[i][j] == 0)
                {
                    Result.emplace_back(i * MAP_SIZE + j,target->Move.player == 1 ? 2 : 1 );
                }
            }
        }
    }
    return Result;
}

void MCTS::AddChildNode(Node* target, MoveInfo move)
{
    Node* New = new Node(0, 0, target, move);
    target->children.push_back(New);
    m_AllAlocations.push_back(New);
}

uint32_t MCTS::InRowCount(const std::vector<std::vector<int>>& currentPos, int posX, int posY, int dx, int dy)
{
    uint32_t HitCount = 0;
    int Player = currentPos[posY][posX];
    while (currentPos[posY][posX] == Player)
    {
        HitCount++;
        if (HitCount >= WIN_CONDITION) 
        {
            break;
        }
        if (posY + dy >= MAP_SIZE || posX + dx >= MAP_SIZE || posX + dx < 0)
        {
            break;
        }
        posY += dy;
        posX += dx;
    }
    return HitCount;
}

size_t MCTS::EmptySpacesOnMap(Node* current)
{
    std::vector<MCTS::MoveInfo> moves = GetValidMoves(current);
    return moves.size();
}

size_t MCTS::EmptySpacesOnMap(std::vector<std::vector<int>> Map)
{
    size_t count = 0;
    for (int i = 0; i < Map.size(); i++)
    {
        for (int j = 0; j < Map[i].size(); j++)
        {
            if (Map[i][j] == 0)
            {
                count++;
            }
        }
    }
    return count;
}